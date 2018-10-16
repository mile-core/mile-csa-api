#include <string>
#include <emscripten/bind.h>
#include "crypto_types.h"
#include "crypto.h"
#include "milecsa_light_api.hpp"

using namespace emscripten;
using milecsa::keys::Pair;

struct error final{
public:
    std::string errorMessage;
    std::string what() const {
        return errorMessage;
    }
};

struct transaction final{
public:
    std::string t_body;
    std::string t_digest;
    std::string body() const {
        return t_body;
    }
    std::string digest() const {
        return t_digest;
    }
};

std::string getPrivateKey(const Pair& p) {
    return p.get_private_key().encode();
}

void setPrivateKey(Pair& p, const std::string& privateKey) {
    p.FromPrivateKey(privateKey);
}

std::string getPublicKey(const Pair& p) {
    return p.get_public_key().encode();
}


EMSCRIPTEN_BINDINGS(mile) {
        function("pair_random", optional_override([](Pair& p, error& error_) {
            return milecsa::keys::generate(p,error_.errorMessage);
        }));

        function("pair_from_private_key", optional_override([](Pair& p,  const std::string& private_key,  error& error_){
            return milecsa::keys::generate_from_private_key(p,private_key,error_.errorMessage);
        }));

        function("pair_with_secret_phrase", optional_override([](Pair& p,  const std::string& phrase, error& error_) {
            return  milecsa::keys::generate_with_secret(p,phrase,error_.errorMessage);
        }));

        function("pair_validate", optional_override([](Pair& p, error& error_) {
            return milecsa::keys::validate(p,error_.errorMessage);
        }));

        function("pair_validate_public_key", optional_override([](const std::string& public_key,  error& error_){
            return milecsa::keys::validate_public_key(public_key,error_.errorMessage);
        }));

        function("pair_validate_private_key", optional_override([](const std::string& private_key, error& error_) {
            return  milecsa::keys::validate_private_key(private_key,error_.errorMessage);
        }));

        function("prepare_transfer", optional_override([](
        const Pair& p,
        const std::string &dstWalletPublicKey,

        const std::string &blockId,
        const std::string &transactionId,
        unsigned short asset,
        const std::string &amount,
        const std::string &description,
        const std::string &fee,
        transaction &transaction,
        error& error_) {

            uint256_t bid;
            uint64_t tid;
            StringToUInt256(blockId, bid, false);
            StringToUInt64(transactionId, tid, false);

            return  milecsa::transaction::prepare_transfer(p,
                                                    dstWalletPublicKey,
                                                    bid,
                                                    tid,
                                                    asset,
                                                    amount,
                                                    description,
                                                    fee,
                                                    transaction.t_body,
                                                    transaction.t_digest,
                                                    error_.errorMessage);
        }));

        function("prepare_emission", optional_override([](
        const Pair& p,
        const std::string &dstWalletPublicKey,

        const std::string &blockId,
        const std::string &transactionId,
        unsigned short asset,
        const std::string &amount,
        const std::string &description,
        const std::string &fee,
        transaction &transaction,
        error& error_) {

            uint256_t bid;
            uint64_t tid;
            StringToUInt256(blockId, bid, false);
            StringToUInt64(transactionId, tid, false);

            return  milecsa::transaction::prepare_emission(p,
                                                           dstWalletPublicKey,
                                                           bid,
                                                           tid,
                                                           asset,
                                                           amount,
                                                           description,
                                                           fee,
                                                           transaction.t_body,
                                                           transaction.t_digest,
                                                           error_.errorMessage);
        }));

        class_<milecsa::keys::Pair>("Pair")
        .constructor<>()
        .class_function("Random", &milecsa::keys::Pair::Random)
        .class_function("FromPrivateKey", &milecsa::keys::Pair::FromPrivateKey)
        .class_function("WithSecret", &milecsa::keys::Pair::WithSecret)
        .property("private_key", getPrivateKey, setPrivateKey)
        .property("public_key", getPublicKey);

        class_<error>("error")
        .constructor<>()
        .property("what", &error::what);

        class_<transaction>("transaction")
        .constructor<>()
        .property("body", &transaction::body)
        .property("digest", &transaction::digest);

        enum_<milecsa::result>("result")
        .value("UNKNOWN", milecsa::result::UNKNOWN)
        .value("OK", milecsa::result::OK)
        .value("FAIL", milecsa::result::FAIL)
        .value("EMPTY", milecsa::result::EMPTY)
        .value("NOT_SUPPORTED", milecsa::result::NOT_SUPPORTED)
        .value("NOT_FOUND", milecsa::result::NOT_FOUND)
        .value("ALREADY_EXIST", milecsa::result::ALREADY_EXIST)
        .value("EXCEPTION", milecsa::result::EXCEPTION)
        .value("LAST", milecsa::result::LAST)
        ;
}