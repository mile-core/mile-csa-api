//
// Created by lotus mile on 03/10/2018.
//

#ifndef MILECSA_MILECSA_EMISSION_HPP
#define MILECSA_MILECSA_EMISSION_HPP

#include "milecsa_transfer.hpp"

namespace milecsa{
    namespace transaction{

        template <typename T, const char name[]>
        class BaseEmission: public Request<T> {
        public:

            using Request<T>::RequestBuilder::RequestBuilder;

            /**
             * Create transfer request object
             *
             * @param keyPair - owner wallet pair
             * @param blockId - current blockchain id
             * @param transactionId - user defined transaction id
             * @param asset - asset
             * @param fee - fee
             * @param error - error closure
             * @return optional request object
             */
            static std::optional<BaseTransfer<T,name>> CreateRequest(const milecsa::keys::Pair &keyPair,
                                                                     const uint256_t blockId,
                                                                     const uint64_t  transactionId,
                                                                     const milecsa::token &asset,
                                                                     float fee = 0,
                                                                     const milecsa::ErrorHandler &error = default_error_handler
            ){

                if (asset.code == milecsa::assets::NILL.code) {
                    error(milecsa::result::NOT_SUPPORTED,
                          ErrorFormat("MILE asset must be %s or %s",  milecsa::assets::XDR.name.c_str(),milecsa::assets::MILE.name.c_str()));
                    return std::nullopt;
                }

                auto request = BaseTransfer<T,name>(keyPair, blockId, transactionId, asset);
                auto fee_string = asset.value_to_string(fee);

                request.setup(
                        name,

                        [&](milecsa::keys::DigestCalculator &calculator){},

                        [&](T &parameters,
                            const std::string &publicKey) {
                            parameters["from"] = publicKey;
                            parameters["asset"] = {{"code", asset.code}};
                            parameters["fee"] = fee_string;
                        });

                return std::make_optional(request);
            }
        };

        static const char  __emissionTransferName[] = "EmissionTransaction";

        /**
         * Emission request builder.
         * @tparam T
         */
        template <typename T>
        class Emission: public BaseEmission<T,__emissionTransferName> {};

        /**
        * Json Emission request type
        */
        typedef milecsa::transaction::Emission<nlohmann::json> JsonEmission;
    }
}

#endif //MILECSA_MILECSA_EMISSION_HPP
