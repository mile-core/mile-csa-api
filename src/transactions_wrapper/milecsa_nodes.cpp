//
// Created by lotus mile on 02/10/2018.
//

#include "milecsa_light_api.hpp"
#include "milecsa_node.hpp"

using json = nlohmann::json;

milecsa::result milecsa::transaction::prepare_register_node(const milecsa::keys::Pair &keyPair,
                                                            const std::string &nodeAddress,

                                                            const uint256_t blockId,
                                                            const uint64_t  transactionId,
                                                            unsigned short assetCode,
                                                            const std::string &amount,
        //
        // Signed json
        //
                                                            std::string &transaction,
                                                            std::string &digest,
                                                            std::string &errorMessage)
{
    milecsa::result result;

    auto error = [&errorMessage, &result](milecsa::result code, const std::string &error) mutable -> void {
        result = code;
        errorMessage = error;
    };

    if(auto node = milecsa::transaction::Node<json>::CreateRegisterRequest(
            keyPair,
            nodeAddress,
            blockId,
            transactionId,
            assetCode,
            amount,
            error)){

        if (auto trx = node->get_body()) transaction = trx->dump();
        if (auto dgst = node->get_digest()) digest = *dgst;

        return milecsa::result::OK;
    }

    return result;
}

milecsa::result milecsa::transaction::prepare_unregister_node(const milecsa::keys::Pair &keyPair,
                                                              const std::string &nodeAddress,

                                                              const uint256_t blockId,
                                                              const uint64_t  transactionId,

                                                              std::string &transaction,
                                                              std::string &digest,
                                                              std::string &errorMessage)
{
    milecsa::result result;

    auto error = [&errorMessage, &result](milecsa::result code, const std::string &error) mutable -> void {
        result = code;
        errorMessage = error;
    };

    if(auto node = milecsa::transaction::Node<json>::CreateUnregisterRequest(
            keyPair,
            nodeAddress,
            blockId,
            transactionId,
            error)){

        if (auto trx = node->get_body()) transaction = trx->dump();
        if (auto dgst = node->get_digest()) digest = *dgst;

        return milecsa::result::OK;
    }

    return result;
}
