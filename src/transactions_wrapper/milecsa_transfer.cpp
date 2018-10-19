//
// Created by lotus mile on 30/09/2018.
//

#include "milecsa_light_api.hpp"
#include "milecsa_transfer.hpp"

using json = nlohmann::json;

milecsa::light::result milecsa::transaction::prepare_transfer(const std::string &privateKey,
													   const std::string &dstWalletPublicKey,

													   const std::string &blockId,
													   const uint64_t  transactionId,

													   unsigned short assetCode,
													   const std::string &amount,
													   const std::string &description,
													   const std::string &fee,
//
// Signed json
//
													   std::string &transaction,
													   std::string &digest,
													   std::string &errorMessage) {
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
    if (auto transfer = milecsa::transaction::Transfer<json>::CreateRequest(
            *keyPair,
            dstWalletPublicKey,
            bid,
            trx_id,
            assetCode,
            amount,
            description,
            fee,
            error)) {

        if (auto trx = transfer->get_body()) transaction = trx->dump();
        if (auto dgst = transfer->get_digest()) digest = *dgst;

        return milecsa::light::result::OK;
    }

    return result;
}
