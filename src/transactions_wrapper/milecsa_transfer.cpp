//
// Created by lotus mile on 30/09/2018.
//

#include "milecsa_light_api.hpp"
#include "milecsa_transfer.hpp"

using json = nlohmann::json;

milecsa::result milecsa::transaction::prepare_transfer(const milecsa::keys::Pair &keyPair,
													   const std::string &dstWalletPublicKey,

													   const uint256_t blockId,
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
    milecsa::result result;

    auto error = [&errorMessage, &result](milecsa::result code, const std::string &error) mutable -> void {
        result = code;
        errorMessage = error;
    };

    if (auto transfer = milecsa::transaction::Transfer<json>::CreateRequest(
            keyPair,
            dstWalletPublicKey,
            blockId,
            transactionId,
            assetCode,
            amount,
            description,
            fee,
            error)) {

        if (auto trx = transfer->get_body()) transaction = trx->dump();
        if (auto dgst = transfer->get_digest()) digest = *dgst;

        return milecsa::result::OK;
    }

    return result;
}