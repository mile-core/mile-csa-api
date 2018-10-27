//#include "milecsa_light_api.hpp"
//
//#include <cstring>
//#include <string>
//#include <string.h>
//
//#include "mile_crypto.h"
//#include "milecsa_result.hpp"
//#include "milecsa_pair.hpp"
//
//milecsa::light::result milecsa::keys::generate(
//        milecsa::light::Pair &keysPair,
//        std::string &errorMessage) {
//
//    auto result = milecsa::light::result::FAIL;
//    if (auto pair =
//            milecsa::keys::Pair::Random([&errorMessage, &result](
//                    milecsa::result code,
//                    std::string error) mutable -> void {
//                result = (milecsa::light::result)code;
//                errorMessage = error;
//            }))
//    {
//        keysPair.private_key = pair->get_private_key().encode();
//        keysPair.public_key = pair->get_public_key().encode();
//        return milecsa::light::result::OK;
//    }
//    else {
//        return result;
//    }
//}
//
//milecsa::light::result milecsa::keys::generate_from_private_key(
//        milecsa::light::Pair &keysPair,
//        const std::string &privateKey,
//        std::string &errorMessage)
//{
//
//    auto result = milecsa::light::result::FAIL;
//    if (auto pair =
//            milecsa::keys::Pair::FromPrivateKey(privateKey, [&errorMessage, &result](
//                    milecsa::result code,
//                    const std::string &error) mutable -> void {
//                result = (milecsa::light::result)code;
//                errorMessage = error;
//            }))
//    {
//        keysPair.private_key = pair->get_private_key().encode();
//        keysPair.public_key = pair->get_public_key().encode();
//        return milecsa::light::result::OK;
//    }
//    else {
//        return result;
//    }
//}
//
//milecsa::light::result milecsa::keys::generate_with_secret(
//        milecsa::light::Pair &keysPair,
//        const std::string &phrase,
//        std::string &errorMessage)
//{
//
//    auto result = milecsa::light::result::FAIL;
//    if (auto pair =
//            milecsa::keys::Pair::WithSecret(phrase, [&errorMessage, &result](
//                    milecsa::result code,
//                    const std::string &error) mutable -> void {
//                result = (milecsa::light::result)code;
//                errorMessage = error;
//            }))
//    {
//        keysPair.private_key = pair->get_private_key().encode();
//        keysPair.public_key = pair->get_public_key().encode();
//        return milecsa::light::result::OK;
//    }
//    else {
//        return result;
//    }
//}
//
//milecsa::light::result milecsa::keys::validate_public_key(
//        const std::string &publicKey,
//        std::string &errorMessage){
//
//    milecsa::light::result result;
//
//    if (milecsa::keys::Pair::ValidatePublicKey(publicKey, [&errorMessage, &result](
//            milecsa::result code,
//            const std::string &error) mutable -> void {
//
//        result = (milecsa::light::result)code;
//        errorMessage = error;
//
//    })) {
//        return  milecsa::light::result::OK;
//    }
//    else {
//        return result;
//    }
//}
//
//
//milecsa::light::result milecsa::keys::validate_private_key(const std::string &privateKey, std::string &errorMessage){
//
//    milecsa::light::result result;
//
//    if (milecsa::keys::Pair::ValidatePrivateKey(privateKey, [&errorMessage, &result](
//            milecsa::result code,
//            const std::string &error) mutable -> void {
//
//        result = (milecsa::light::result)code;
//        errorMessage = error;
//
//    })) {
//        return  milecsa::light::result::OK;
//    }
//    else {
//        return result;
//    }
//}
//
//milecsa::light::result milecsa::keys::validate(const milecsa::light::Pair &keysPair, std::string &errorMessage){
//
//    milecsa::light::result result = milecsa::keys::validate_public_key(keysPair.public_key, errorMessage);
//    if (result != milecsa::light::result::OK)
//        return result;
//    return milecsa::keys::validate_private_key(keysPair.private_key, errorMessage);
//}
