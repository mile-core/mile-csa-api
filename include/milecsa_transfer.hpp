//
// Created by lotus mile on 03/10/2018.
//

#ifndef MILECSA_MILESCA_TRANSFER_HPP
#define MILECSA_MILESCA_TRANSFER_HPP

#include "milecsa_transaction.hpp"
#include <string>
#include <functional>

namespace milecsa{
    namespace transaction{

        /**
         * Transfer builder
         * @tparam T
         */
        template <typename T, const char name[]>
        class BaseTransfer: public Request<T> {
        public:

            using Request<T>::RequestBuilder::RequestBuilder;

            /**
             * Create transfer request object
             *
             * @param keyPair - owner wallet pair
             * @param dstWalletPublicKey - destination address
             * @param blockId - current blockchain id
             * @param transactionId - user defined transaction id
             * @param assetCode - asset code
             * @param amount - how much
             * @param description - transfer description
             * @param fee - fee, can be empty
             * @param error - error closure
             * @return optional request object
             */
            static std::optional<BaseTransfer<T,name>> CreateRequest(const milecsa::keys::Pair &keyPair,
                                                                     const std::string &dstWalletPublicKey,

                                                                     const uint256_t blockId,
                                                                     const uint64_t  transactionId,
                                                                     const milecsa::token &asset,
                                                                     float amount,
                                                                     float fee = 0,
                                                                     const std::string &description = "",
                                                                     const milecsa::ErrorHandler &error = default_error_handler
            ){

                auto amount_string = asset.value_to_string(amount);

                if (amount <=0.0f || amount_string.empty()) {
                    error(milecsa::result::EMPTY,
                          ErrorFormat("MILE amount must be over then 0"));
                    return std::nullopt;
                }

                if (keyPair.get_public_key().encode() == dstWalletPublicKey) {
                    error(milecsa::result::ALREADY_EXIST,
                          ErrorFormat("destination address is equal the source"));
                    return std::nullopt;
                }

                milecsa::keys::PublicKey destination;
                std::string errorMessage;
                if(!destination.decode(dstWalletPublicKey, errorMessage)) {
                    error(milecsa::result::FAIL, ErrorFormat(errorMessage.c_str()));
                    return std::nullopt;
                }

                auto request = BaseTransfer<T,name>(keyPair, blockId, transactionId, asset);
                auto fee_string = asset.value_to_string(fee);

                request.setup(
                        name,

                        [&](milecsa::keys::DigestCalculator &calculator){
                            calculator.Update(destination);
                            calculator.Update(amount_string, amount_string.size());
                            calculator.Update(description, description.size());
                        },

                        [&](T &parameters,
                            const std::string &publicKey) {
                            parameters["from"] = publicKey;
                            parameters["to"] = destination.encode();
                            parameters["asset"] = {{"amount", amount_string}, {"code", asset.code}};
                            parameters["fee"] = fee_string;
                            parameters["description"] = description;

                        });

                return std::make_optional(request);
            }
        };

        static const char  __transferName[] = "TransferAssetsTransaction";
        template <typename T>
        class Transfer: public BaseTransfer<T, __transferName> {};
    }
}

#endif //MILECSA_MILESCA_TRANSFER_HPP
