//
// Created by lotus mile on 02/10/2018.
//

#include "milecsa_light_api.hpp"
#include "milecsa_voting.hpp"

using json = nlohmann::json;

milecsa::light::result milecsa::transaction::prepare_vote_for_asstes_rate(const std::string &privateKey,

                                                                   const std::string &blockId,
                                                                   const uint64_t  transactionId,

                                                                   unsigned short assetCode,
                                                                   const std::string &rate,

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

    if(auto emission = milecsa::transaction::Vote<json>::CreateRequest(
            *keyPair,
            bid,
            trx_id,
            assetCode,
            rate,
            error)){

        if (auto trx = emission->get_body()) transaction = trx->dump();
        if (auto dgst = emission->get_digest()) digest = *dgst;

        return milecsa::light::result::OK;
    }

    return result;
}
