#include <string>
#include <emscripten/bind.h>
#include "crypto_types.h"
#include "crypto.h"
#include "milecsa_light_api.hpp"

using namespace emscripten;
using milecsa::light::Pair;
using milecsa::light::result;

struct error final{
public:
    std::string errorMessage;
    std::string what() const {
        return errorMessage;
    }
    bool hasError() const {
        return !errorMessage.empty();
    }
};

struct Transaction final{
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
    return p.private_key;
}

void setPrivateKey(Pair& p, const std::string& privateKey) {
    //p.FromPrivateKey(privateKey);
    std::string error;
    milecsa::keys::generate_from_private_key(p, privateKey, error);
}

std::string getPublicKey(const Pair& p) {
    return p.public_key;
}


EMSCRIPTEN_BINDINGS(mile) {

        class_<milecsa::light::Pair>("Pair")
        .constructor<>()

        .property("private_key", getPrivateKey, setPrivateKey)
        .property("public_key", getPublicKey)

        .class_function("Random", optional_override([](error &error_) {
            error_.errorMessage = "";
            Pair *ptr = new Pair();
            if(milecsa::keys::generate(*ptr,error_.errorMessage) != result::OK){
                delete ptr;
                ptr = nullptr;
                return ptr;
            }
            return ptr;
        }),allow_raw_pointers())

        .class_function("WithSecret", optional_override([](const std::string &phrase, error &error_) {
            error_.errorMessage = "";
            Pair *ptr = new Pair();
            if(milecsa::keys::generate_with_secret(*ptr, phrase, error_.errorMessage) != result::OK){
                delete ptr;
                ptr = nullptr;
                return ptr;
            }
            return ptr;
        }),allow_raw_pointers())

        .class_function("FromPrivateKey", optional_override([](const std::string &pk, error &error_) {
            error_.errorMessage = "";
            Pair *ptr = new Pair();
            if(milecsa::keys::generate_from_private_key(*ptr, pk, error_.errorMessage) != result::OK){
                delete ptr;
                ptr = nullptr;
                return ptr;
            }
            return ptr;
        }),allow_raw_pointers())

        .class_function("Validate", optional_override([](const Pair &p, error &error_) {
            error_.errorMessage = "";
            return milecsa::keys::validate(p,error_.errorMessage) == result::OK;
        }))

        .class_function("ValidatePublicKey", optional_override([](const std::string &public_key, error &error_) {
            error_.errorMessage = "";
            return milecsa::keys::validate_public_key(public_key,error_.errorMessage) == result::OK;
        }))

        .class_function("ValidatePrivateKey", optional_override([](const std::string &private_key, error &error_) {
            error_.errorMessage = "";
            return milecsa::keys::validate_private_key(private_key,error_.errorMessage) == result::OK;
        }))
        ;

        class_<error>("Error")
        .constructor<>()
        .property("hasError", &error::hasError)
        .property("what", &error::what);

        class_<Transaction>("Transaction")

        .constructor<>()

        .property("body", &Transaction::body)
        .property("digest", &Transaction::digest)

        .function("Transfer", optional_override([](
        Transaction& transaction,
        const Pair& p,
        const std::string &dstWalletPublicKey,
        const std::string &blockId,
        const std::string &transactionId,
        unsigned short asset,
        const std::string &amount,
        const std::string &description,
        const std::string &fee,
        error& error_) {
            error_.errorMessage = "";

            //uint256_t bid;
            uint64_t tid;
            //StringToUInt256(blockId, bid, false);
            StringToUInt64(transactionId, tid, false);

            error_.errorMessage = "";

            return  milecsa::transaction::prepare_transfer(p.private_key,
                                                           dstWalletPublicKey,
                                                           blockId,
                                                           tid,
                                                           asset,
                                                           amount,
                                                           description,
                                                           fee,
                                                           transaction.t_body,
                                                           transaction.t_digest,
                                                           error_.errorMessage);
        }))
        .function("Emission", optional_override([](
        Transaction& transaction,
        const Pair& p,
        const std::string &dstWalletPublicKey,
        const std::string &blockId,
        const std::string &transactionId,
        unsigned short asset,
        const std::string &amount,
        const std::string &description,
        const std::string &fee,
        error& error_) {
            error_.errorMessage = "";

           // uint256_t bid;
            uint64_t tid;
           // StringToUInt256(blockId, bid, false);
            StringToUInt64(transactionId, tid, false);

            error_.errorMessage = "";

            return  milecsa::transaction::prepare_emission(p.private_key,
                                                           dstWalletPublicKey,
                                                           blockId,
                                                           tid,
                                                           asset,
                                                           amount,
                                                           description,
                                                           fee,
                                                           transaction.t_body,
                                                           transaction.t_digest,
                                                           error_.errorMessage);
        }));


        enum_<result>("result")
        .value("UNKNOWN", result::UNKNOWN)
        .value("OK", result::OK)
        .value("FAIL", result::FAIL)
        .value("EMPTY", result::EMPTY)
        .value("NOT_SUPPORTED", result::NOT_SUPPORTED)
        .value("NOT_FOUND", result::NOT_FOUND)
        .value("ALREADY_EXIST", result::ALREADY_EXIST)
        .value("EXCEPTION", result::EXCEPTION)
        .value("LAST", result::LAST)
        ;
}