//
// Created by lotus mile on 02/10/2018.
//

#include "milecsa_light_api.hpp"
#include "milecsa_voting.hpp"

using json = nlohmann::json;

milecsa::result milecsa::transaction::prepare_vote_for_asstes_rate(const milecsa::keys::Pair &keyPair,

                                                                 const uint256_t blockId,
                                                                 const uint64_t  transactionId,

                                                                 unsigned short assetCode,
                                                                 const std::string &rate,

                                                                 std::string &transaction,
                                                                 std::string &digest,
                                                                 std::string &errorMessage)
{
    milecsa::result result;

    auto error = [&errorMessage, &result](milecsa::result code, const std::string &error) mutable -> void {
        result = code;
        errorMessage = error;
    };

    if(auto emission = milecsa::transaction::Vote<json>::CreateRequest(
            keyPair,
            blockId,
            transactionId,
            assetCode,
            rate,
            error)){

        if (auto trx = emission->get_body()) transaction = trx->dump();
        if (auto dgst = emission->get_digest()) digest = *dgst;

        return milecsa::result::OK;
    }

    return result;
}
