//
// Created by lotus mile on 02/10/2018.
//

#include "milecsa_light_api.hpp"
#include "milecsa_node.hpp"

using json = nlohmann::json;

milecsa::light::result milecsa::transaction::prepare_register_node(const std::string &privateKey,
                                                            const std::string &nodeAddress,

                                                            const std::string &blockId,
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
    milecsa::light::result result;

    auto error = [&errorMessage, &result](milecsa::result code, const std::string &error) mutable -> void {
        result = (milecsa::light::result)code;
        errorMessage = error;
    };

    auto keyPair = milecsa::keys::Pair::FromPrivateKey(privateKey, error);
    if (!keyPair)
        return milecsa::light::result::FAIL;

    uint256_t bid;

    if(!StringToUInt256(blockId, bid, false)) {
        errorMessage = "block could not be converted to uint256_t";
        return milecsa::light::result::FAIL;
    }

    uint64_t trx_id = transactionId == default_transaction_id ? (uint64_t)rand() : transactionId;

    if(auto node = milecsa::transaction::Node<json>::CreateRegisterRequest(
            *keyPair,
            nodeAddress,
            bid,
            trx_id,
            assetCode,
            amount,
            error)){

        if (auto trx = node->get_body()) transaction = trx->dump();
        if (auto dgst = node->get_digest()) digest = *dgst;

        return milecsa::light::result::OK;
    }

    return result;
}

milecsa::light::result milecsa::transaction::prepare_unregister_node(const std::string &privateKey,
                                                              const std::string &nodeAddress,

                                                              const std::string &blockId,
                                                              const uint64_t  transactionId,

                                                              std::string &transaction,
                                                              std::string &digest,
                                                              std::string &errorMessage)
{
    milecsa::light::result result;

    auto error = [&errorMessage, &result](milecsa::result code, const std::string &error) mutable -> void {
        result = (milecsa::light::result)code;
        errorMessage = error;
    };

    auto keyPair = milecsa::keys::Pair::FromPrivateKey(privateKey, error);
    if (!keyPair)
        return milecsa::light::result::FAIL;

    uint256_t bid;

    if(!StringToUInt256(blockId, bid, false)) {
        errorMessage = "block could not be converted to uint256_t";
        return milecsa::light::result::FAIL;
    }

    uint64_t trx_id = transactionId == default_transaction_id ? (uint64_t)rand() : transactionId;

    if(auto node = milecsa::transaction::Node<json>::CreateUnregisterRequest(
            *keyPair,
            nodeAddress,
            bid,
            trx_id,
            error)){

        if (auto trx = node->get_body()) transaction = trx->dump();
        if (auto dgst = node->get_digest()) digest = *dgst;

        return milecsa::light::result::OK;
    }

    return result;
}
