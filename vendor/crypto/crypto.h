// hash functions and Ed25519 library primitives
#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>
#include <vector>
extern "C" {
#include "sha3.h"
}
#include "crypto_types.h"

using std::vector;
using std::ostream;

// hash functions

// calculate SHA256 hash
// note: check if sha3 implementation is correct for big endian CPUs
void CalculateDigest(const vector<unsigned char>& message, Digest& digest);

// cryptographic primitives for Ed25519 library
// note: SHA512 is used for message hashing

void CreateKeyPair(PrivateKey& privateKey, PublicKey& publicKey, Seed& seed);

void CreateKeyPair(PrivateKey& privateKey, PublicKey& publicKey);

void RestoreKeyPairFromPrivate(const PrivateKey& privateKey, PublicKey& publicKey);

// note: why ed25519 uses public key for signing?
void SignMessage(const vector<unsigned char>& message, const PrivateKey& privateKey, const PublicKey& publicKey, Signature& signature);

bool VerifySignature(const vector<unsigned char>& message, const Signature& signature, const PublicKey& publicKey);

bool VerifySignature(const Digest& digest, const Signature& signature, const PublicKey& publicKey);

class DigestCalculator
{
    sha3_context m_ctx;
public:
    // initialize
    DigestCalculator();
    // initialize
    void Initialize();
    //
    void Update(const vector<unsigned char>& message);
    //
    template<size_t N>
    void Update(const Array<N>& array)
    {
        sha3_Update(&m_ctx, array.Data.data(), array.Data.size());
    }
    //
    void Update(bool value);
    void Update(unsigned char value);
    // use little endian by default
    void Update(unsigned short value, bool littleEndian = true);
    void Update(const uint64_t& value, bool littleEndian = true);
    // use little endian by default
    void Update(unsigned int value, bool littleEndian = true);
    // use little endian by default
    void Update(const uint256_t& value, bool littleEndian = true);
    
    // string data will be padded if size > string size
    void Update(const string& s, size_t size = 0);
    //
    void Finalize(Digest& digest);
};

class Signer
{
    PrivateKey m_privateKey;
    PublicKey  m_publicKey;
public:
    Signer();
    Signer(const Signer& signer);
    Signer(const PrivateKey& privateKey, const PublicKey& publicKey);
    ~Signer();
    //
    void GenerateRandomKeys();
    void Set(const PrivateKey& privateKey, const PublicKey& publicKey);
    //
    const PublicKey& GetPublicKey() const { return m_publicKey; }
    //
    void SignMessage(const vector<unsigned char>& message, Signature& signature) const;
    void SignDigest(const Digest& digest, Signature& signature) const;
    bool VerifySignature(const vector<unsigned char>& message, const Signature& signature) const;
    bool VerifySignature(const Digest& digest, const Signature& signature) const;
    //
    // ToString() ?
    void Dump(ostream& outputStream);
    //
    Signer& operator= (const Signer& other);
};

#endif
