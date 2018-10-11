#include "milecsa_light_api.hpp"

#include <cstring>
#include <string>
#include <string.h>

#include "ge.h"
#include "crypto_types.h"
#include "crypto.h"
#include "milecsa_result.hpp"
#include "milecsa_utils.hpp"

milecsa::result milecsa::keys::generate(
        milecsa::keys::Pair &keysPair,
        std::string &errorMessage) {

    auto result = milecsa::result::FAIL;
    if (auto pair =
            milecsa::keys::Pair::Random([&errorMessage, &result](
                    milecsa::result code,
                    std::string error) mutable -> void {
                result = code;
                errorMessage = error;
            }))
    {
        keysPair = *pair;
        return milecsa::result::OK;
    }
    else {
        return result;
    }
}

milecsa::result milecsa::keys::generate_from_private_key(
        milecsa::keys::Pair &keysPair,
        const std::string &privateKey,
        std::string &errorMessage)
{

    auto result = milecsa::result::FAIL;
    if (auto pair =
            milecsa::keys::Pair::FromPrivateKey(privateKey, [&errorMessage, &result](
                    milecsa::result code,
                    const std::string &error) mutable -> void {
                result = code;
                errorMessage = error;
            }))
    {
        keysPair = *pair;
        return milecsa::result::OK;
    }
    else {
        return result;
    }
}

milecsa::result milecsa::keys::generate_with_secret(
        milecsa::keys::Pair &keysPair,
        const std::string &phrase,
        std::string &errorMessage)
{

    auto result = milecsa::result::FAIL;
    if (auto pair =
            milecsa::keys::Pair::WithSecret(phrase, [&errorMessage, &result](
                    milecsa::result code,
                    const std::string &error) mutable -> void {
                result = code;
                errorMessage = error;
            }))
    {
        keysPair = *pair;
        return milecsa::result::OK;
    }
    else {
        return result;
    }
}

milecsa::result milecsa::keys::validate_public_key(
        const std::string &publicKey,
        std::string &errorMessage){

    milecsa::result result;

    if (milecsa::keys::Pair::ValidatePublicKey(publicKey, [&errorMessage, &result](
            milecsa::result code,
            const std::string &error) mutable -> void {

        result = code;
        errorMessage = error;

    })) {
        return  milecsa::result::OK;
    }
    else {
        return result;
    }
}


milecsa::result milecsa::keys::validate_private_key(const std::string &privateKey, std::string &errorMessage){

    milecsa::result result;

    if (milecsa::keys::Pair::ValidatePrivateKey(privateKey, [&errorMessage, &result](
            milecsa::result code,
            const std::string &error) mutable -> void {

        result = code;
        errorMessage = error;

    })) {
        return  milecsa::result::OK;
    }
    else {
        return result;
    }
}

milecsa::result milecsa::keys::validate(const milecsa::keys::Pair &keysPair, std::string &errorMessage){

    milecsa::result result;

    if (milecsa::keys::Pair::Validate(keysPair, [&errorMessage, &result](
            milecsa::result code,
            const std::string &error) mutable -> void {

        result = code;
        errorMessage = error;

    })) {
        return  milecsa::result::OK;
    }
    else {
        return result;
    }
}
