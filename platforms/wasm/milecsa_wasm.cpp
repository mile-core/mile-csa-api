#include <string>
#include <emscripten/bind.h>
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
    function("generate_key_pair", optional_override([](Pair& p, error& error_) {
        return milecsa::keys::generate(p,error_.errorMessage);
    }));

    function("generate_key_pair_from_private_key", optional_override([](Pair& p,  const std::string& private_key,  error& error_){
        return milecsa::keys::generate_from_private_key(p,private_key,error_.errorMessage);
    }));

    function("generate_key_pair_with_secret_phrase", optional_override([](Pair& p,  const std::string& phrase, error& error_) {
        return  milecsa::keys::generate_with_secret(p,phrase,error_.errorMessage);
    }));

    function("validate", optional_override([](Pair& p, error& error_) {
            return milecsa::keys::validate(p,error_.errorMessage);
    }));

    function("validate_public_key", optional_override([](const std::string& public_key,  error& error_){
            return milecsa::keys::validate_public_key(public_key,error_.errorMessage);
    }));

    function("validate_private_key", optional_override([](const std::string& private_key, error& error_) {
            return  milecsa::keys::validate_private_key(private_key,error_.errorMessage);
    }));

    class_<milecsa::keys::Pair>("keys_pair")
            .constructor<>()
            .property("private_key", getPrivateKey, setPrivateKey)
            .property("public_key", getPublicKey);

    class_<error>("error")
            .constructor<>()
            .property("what", &error::what);

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