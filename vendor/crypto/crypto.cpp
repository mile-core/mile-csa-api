#include "ed25519.h"
#include "ed25519_ext.h"
#include "crypto.h"
#include "crypto_types.h"

// ----- hash -----

void CalculateDigest(const vector<unsigned char>& message, Digest& digest)
{
    sha3_256(message.data(), message.size(), digest.Data.data());
}

// ----- Ed25519 -----

void CreateKeyPair(PrivateKey& privateKey, PublicKey& publicKey, Seed& seed)
{
    ed25519_create_keypair(publicKey.Data.data(), privateKey.Data.data(), seed.Data.data());
}

void CreateKeyPair(PrivateKey& privateKey, PublicKey& publicKey)
{
    Seed seed;
    // generate default seed
    ed25519_create_seed(seed.Data.data());
    // create keypair
    ed25519_create_keypair(publicKey.Data.data(), privateKey.Data.data(), seed.Data.data());
}

void RestoreKeyPairFromPrivate(const PrivateKey& privateKey, PublicKey& publicKey)
{
    ed25519_restore_from_private_key(publicKey.Data.data(), privateKey.Data.data());
}

void SignMessage(const vector<unsigned char>& message, const PrivateKey& privateKey, const PublicKey& publicKey, Signature& signature)
{
    ed25519_sign(signature.Data.data(), message.data(), message.size(), publicKey.Data.data(), privateKey.Data.data());
}

bool VerifySignature(const vector<unsigned char>& message, const Signature& signature, const PublicKey& publicKey)
{
    return ed25519_verify(signature.Data.data(), message.data(), message.size(), publicKey.Data.data()) == 1;
}

bool VerifySignature(const Digest& digest, const Signature& signature, const PublicKey& publicKey)
{
    return ed25519_verify(signature.Data.data(), digest.Data.data(), digest.Size(), publicKey.Data.data()) == 1;
}

// ----- DigestCalculator -----

// initialize digest calculator
DigestCalculator::DigestCalculator()
{
    sha3_Init256(&m_ctx);
}

void DigestCalculator::Initialize()
{
    sha3_Init256(&m_ctx);
}

void DigestCalculator::Update(const vector<unsigned char>& message)
{
    sha3_Update(&m_ctx, message.data(), message.size());
}

void DigestCalculator::Update(bool value)
{
    unsigned char data = value ? 1 : 0; // platform-independent
    sha3_Update(&m_ctx, &data, 1);      // 1 byte
}

void DigestCalculator::Update(unsigned char value)
{
    sha3_Update(&m_ctx, (void const *)&value, 1); // 1 byte
}

void DigestCalculator::Update(unsigned short value, bool littleEndian)
{
    vector<unsigned char> message;
    //
    if (littleEndian)
    {
        message.push_back(static_cast<unsigned char>(value & 0xff));
        message.push_back(static_cast<unsigned char>((value >> 8) & 0xff));
    }
    else // big endian
    {
        message.push_back(static_cast<unsigned char>((value >> 8) & 0xff));
        message.push_back(static_cast<unsigned char>(value & 0xff));
    }
    //
    sha3_Update(&m_ctx, message.data(), message.size());
}

void DigestCalculator::Update(const uint64_t& value, bool littleEndian)
{
    vector<unsigned char> message;
    //
    UInt64ToVector(value, message, littleEndian);
    sha3_Update(&m_ctx, message.data(), message.size());
}

void DigestCalculator::Update(const uint256_t& value, bool littleEndian)
{
    vector<unsigned char> message;
    //
    UInt256ToVector(value, message, littleEndian);
    sha3_Update(&m_ctx, message.data(), message.size());
}

void DigestCalculator::Update(unsigned int value, bool littleEndian)
{
    vector<unsigned char> message;
    //
    if (littleEndian)
    {
        message.push_back(static_cast<unsigned char>(value & 0xff));
        message.push_back(static_cast<unsigned char>((value >> 8) & 0xff));
        message.push_back(static_cast<unsigned char>((value >> 16) & 0xff));
        message.push_back(static_cast<unsigned char>((value >> 24) & 0xff));
    }
    else // big endian
    {
        message.push_back(static_cast<unsigned char>((value >> 24) & 0xff));
        message.push_back(static_cast<unsigned char>((value >> 16) & 0xff));
        message.push_back(static_cast<unsigned char>((value >> 8) & 0xff));
        message.push_back(static_cast<unsigned char>(value & 0xff));
    }
    //
    sha3_Update(&m_ctx, message.data(), message.size());
}

void DigestCalculator::Update(const string& s, size_t size)
{
    if (size == 0)
    {
        sha3_Update(&m_ctx, s.data(), s.size());
    }
    else 
    {
        if (s.size() >= size)
        {
            // cut string data
            sha3_Update(&m_ctx, s.data(), size);
        }
        else // s.size() < size
        {
            // add string padding
            vector<unsigned char> message(s.begin(), s.end());
            message.resize(size);
            sha3_Update(&m_ctx, message.data(), message.size());
        }
    }
}

void DigestCalculator::Finalize(Digest& digest)
{
    sha3_Finalize(&m_ctx, digest.Data.data());
}

// ----- Signer -----

Signer::Signer()
{
    m_privateKey.Clear();
    m_publicKey.Clear();
}

Signer::Signer(const Signer& signer)
    : m_privateKey(signer.m_privateKey)
    , m_publicKey(signer.m_publicKey)
{
}

Signer::Signer(const PrivateKey& privateKey, const PublicKey& publicKey)
    : m_privateKey(privateKey)
    , m_publicKey(publicKey)
{
}

Signer::~Signer()
{
}

void Signer::GenerateRandomKeys()
{
    CreateKeyPair(m_privateKey, m_publicKey);
}

void Signer::Set(const PrivateKey& privateKey, const PublicKey& publicKey)
{
    m_privateKey = privateKey;
    m_publicKey = publicKey;
}

void Signer::SignMessage(const vector<unsigned char>& message, Signature& signature) const
{
    ed25519_sign(signature.Data.data(), message.data(), message.size(), m_publicKey.Data.data(), m_privateKey.Data.data());
}

void Signer::SignDigest(const Digest& digest, Signature& signature) const
{
    ed25519_sign(signature.Data.data(), digest.Data.data(), digest.Data.size(), m_publicKey.Data.data(), m_privateKey.Data.data());
}

bool Signer::VerifySignature(const vector<unsigned char>& message, const Signature& signature) const
{
    return ::VerifySignature(message, signature, m_publicKey);
}

bool Signer::VerifySignature(const Digest& digest, const Signature& signature) const
{
    return ed25519_verify(signature.Data.data(), digest.Data.data(), digest.Size(), m_publicKey.Data.data()) == 1;
}

void Signer::Dump(ostream& outputStream)
{
    outputStream << "signer private key : " << "\n" << m_privateKey.ToBase58CheckString().c_str();
    outputStream << "signer public key  : " << "\n" << m_publicKey.ToBase58CheckString().c_str();
}

Signer& Signer::operator= (const Signer& other)
{
    m_privateKey = other.m_privateKey;
    m_publicKey = other.m_publicKey; 
    return *this;
}
