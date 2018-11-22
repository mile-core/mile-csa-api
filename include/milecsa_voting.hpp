//
// Created by lotus mile on 04/10/2018.
//

#ifndef MILECSA_MILECSA_VOTING_HPP
#define MILECSA_MILECSA_VOTING_HPP

#include "milecsa_transaction.hpp"

namespace milecsa{
    namespace transaction{

        /**
         * Node request builder
         *
         * @tparam T - serealization protocol class. It must support subscript operator.
         */
        template <typename T>
        class Vote: public Request<T> {
        public:

            using Request<T>::RequestBuilder::RequestBuilder;

            static std::optional<Vote<T>> CreateRequest(const milecsa::keys::Pair &keyPair,

                                                        const uint256_t blockId,
                                                        const uint64_t transactionId,

                                                        float xdr_rate,
                                                        float fee = 0,

                                                        const milecsa::ErrorHandler &error = default_error_handler
            ) {

                std::string rate_string =  milecsa::assets::XDR.value_to_string(xdr_rate);

                if (xdr_rate <= 0.0f || rate_string.empty()) {
                    error(milecsa::result::EMPTY,
                          ErrorFormat("MILE rate must be over then 0"));
                    return std::nullopt;
                }

                auto request = Vote<T>(keyPair, blockId, transactionId, milecsa::assets::NILL);
                auto fee_string = milecsa::assets::XDR.value_to_string(fee);

                request.setup(
                        "PostTokenRate",

                        [&](milecsa::keys::DigestCalculator &calculator) {
                            calculator.Update(rate_string, rate_string.size());
                        },

                        [&](T &parameters,
                            const std::string &publicKey) {

                            parameters["public-key"] = publicKey;
                            parameters["asset"] = {{"amount", rate_string},
                                                   {"code",   milecsa::assets::XDR.code}};
                            parameters["fee"] = fee_string;
                        });

                return std::make_optional(request);
            }
        };

        /**
         * Json Vote type
         */
        typedef milecsa::transaction::Vote<nlohmann::json> JsonVote;
    }
}

#endif //MILECSA_MILECSA_VOTING_HPP
