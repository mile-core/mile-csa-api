//
// Created by lotus mile on 03/10/2018.
//

#include "milecsa_pair.hpp"

namespace milecsa::keys{
    void PublicKey::clear() {
        this->Clear();
    }

    const std::string PublicKey::encode() const {
        return this->ToBase58CheckString();
    }

    bool PublicKey::decode(const std::string &key, std::string &error) {
        return this->SetBase58CheckString(key, error);
    }

    void PrivateKey::clear() {
        this->Clear();
    }

    const std::string PrivateKey::encode() const {
        return this->ToBase58CheckString();
    }

    bool PrivateKey::decode(const std::string &key, std::string &error) {
        return this->SetBase58CheckString(key, error);
    }
}

namespace milecsa::keys{

    bool Pair::validate(const ErrorHandler &error) {

        if (public_key_.IsZero()){
            error(milecsa::result::EMPTY, ErrorFormat("public key couldn't be empty"));
            return false;
        }

        if (private_key_.IsZero()){
            error(milecsa::result::EMPTY, ErrorFormat("private key couldn't be empty"));
            return false;
        }

        return true;
    }

    bool Pair::Validate(const milecsa::keys::Pair &pair, const milecsa::ErrorHandler &error) {
        if(!ValidatePrivateKey(pair.private_key_.encode(),error)){
            return false;
        }
        if(!ValidatePublicKey(pair.public_key_.encode(),error)){
            return false;
        }
        return true;
    }

    bool Pair::ValidatePublicKey(const std::string &publicKey, const milecsa::ErrorHandler &error) {
        PublicKey pk;
        return ValidateKey(pk,publicKey,"public",error);
    }

    bool Pair::ValidatePrivateKey(const std::string &privateKey, const milecsa::ErrorHandler &error) {
        PrivateKey pk;
        return ValidateKey(pk,privateKey,"private",error);
    }

    bool Pair::ValidateKey(milecsa::keys::Key &pairKey, const std::string &keyString,
                           const std::string &name, const milecsa::ErrorHandler &error) {

        std::string errorDescription;

        if (!pairKey.decode(keyString, errorDescription))
        {
            error(milecsa::result::FAIL, ErrorFormat("read %s key: %s", name.c_str(), errorDescription.c_str()));
            return false;
        }

        return true;
    }

    std::optional<Pair> Pair::Random(const milecsa::ErrorHandler &error) {

        auto  pair = Pair();
        pair.clear();
        CreateKeyPair(pair.private_key_, pair.public_key_);

        if (!pair.validate(error)){
            return std::nullopt;
        }

        return std::make_optional(pair);
    }

    std::optional<Pair> Pair::WithSecret(const std::string &phrase, const milecsa::ErrorHandler &error) {

        if (phrase.empty())
        {
            error(milecsa::result::EMPTY, ErrorFormat("secret phrase is empty"));
            return std::nullopt;
        }

        Seed seed;
        seed.Clear();
        sha3_256((const unsigned char*) phrase.c_str(), phrase.length(), seed.Data.data());

        auto  pair = Pair();
        pair.clear();
        CreateKeyPair(pair.private_key_, pair.public_key_, seed);

        return std::make_optional(pair);
    }

    std::optional<Pair> Pair::FromPrivateKey(const std::string &privateKey, const milecsa::ErrorHandler &error) {

        if (privateKey.empty())
        {
            error(milecsa::result::EMPTY, ErrorFormat("private key is empty"));
            return std::nullopt;
        }

        auto  pair = Pair();
        pair.clear();

        std::string errorDescription;

        if (!pair.private_key_.decode(privateKey, errorDescription))
        {
            error(milecsa::result::EMPTY, ErrorFormat("Error read private key: %s", errorDescription.c_str()));
            return std::nullopt;
        }

        RestoreKeyPairFromPrivate(pair.private_key_, pair.public_key_);

        return std::make_optional(pair);
    }

}
