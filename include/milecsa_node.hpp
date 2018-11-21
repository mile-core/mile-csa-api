//
// Created by lotus mile on 03/10/2018.
//

#ifndef MILECSA_MILECSA_NODE_HPP
#define MILECSA_MILECSA_NODE_HPP

#include "milecsa_transaction.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace milecsa{
    namespace transaction{

        /**
         * Node request builder
         *
         * @tparam T - serealization protocol class. It must support subscript operator.
         */
        template <typename T>
        class Node: public Request<T> {
        public:

            using Request<T>::RequestBuilder::RequestBuilder;

            /**
              * Create request to register MILE node
              *
              * @param keyPair
              * @param nodeAddress
              * @param blockId
              * @param transactionId
              * @param assetCode
              * @param amount
              * @param error
              * @return Request object
              */
            static std::optional<Node<T>> CreateRegisterRequest(const milecsa::keys::Pair &keyPair,
                                                                const std::string &nodeAddress,

                                                                const uint256_t blockId,
                                                                const uint64_t  transactionId,

                                                                float  amount,
                                                                float fee = 0,
                                                                const milecsa::ErrorHandler &error = default_error_handler
            ){

                auto amount_string = milecsa::assets::XDR.value_to_string(amount);

                if (amount <=0.0f || amount_string.empty()) {
                    error(milecsa::result::EMPTY,
                          ErrorFormat("XDR amount must be over then 0"));
                    return std::nullopt;
                }

                if (nodeAddress.empty()) {
                    error(milecsa::result::EMPTY,
                          ErrorFormat("node address is empty"));
                    return std::nullopt;
                }

                if (nodeAddress == "localhost" || nodeAddress == "127.0.0.1") {
                    error(milecsa::result::NOT_SUPPORTED,
                          ErrorFormat("node address couldn't be localhost"));
                    return std::nullopt;
                }

                struct in_addr addr;
                const char *na = nodeAddress.c_str();

                if (inet_aton(na, &addr) == 0 ) {
                    if (gethostbyname(na) == nullptr){
                        error(milecsa::result::NOT_FOUND,
                              ErrorFormat("address %s is not host name or ip", nodeAddress.c_str()));
                        return std::nullopt;
                    }
                }

                auto request = Node<T>(keyPair, blockId, transactionId, milecsa::assets::NILL);

                auto fee_string = milecsa::assets::XDR.value_to_string(fee);

                request.setup(
                        "RegisterNodeTransactionWithAmount",
                        [&](DigestCalculator &calculator){
                            calculator.Update(nodeAddress, 64);
                            calculator.Update(amount_string, amount_string.size());
                        },

                        [&](T &parameters,
                            const std::string &publicKey) {
                            parameters["public-key"] = publicKey;
                            parameters["asset"] = { { "amount", amount_string }, {"code", milecsa::assets::XDR.code}} ;
                            parameters["address"] = nodeAddress;
                            parameters["fee"] = fee_string;
                        });

                return std::make_optional(request);
            }

            /***
             *
             * Create request to dismiss MILE node
             *
             * @param keyPair
             * @param nodeAddress
             * @param blockId
             * @param transactionId
             * @param error
             * @return
             */
            static std::optional<Node<T>> CreateUnregisterRequest(const milecsa::keys::Pair &keyPair,

                                                                  const uint256_t blockId,
                                                                  const uint64_t  transactionId,
                                                                  float fee = 0,
                                                                  const milecsa::ErrorHandler &error = default_error_handler
            ){

                auto request = Node<T>(keyPair, blockId, transactionId);
                auto fee_string = milecsa::assets::XDR.value_to_string(fee);

                request.setup(
                        "UnregisterNodeTransaction",
                        [&](DigestCalculator &calculator){},

                        [&](T &parameters,
                            const std::string &publicKey) {
                            parameters["public-key"] = publicKey;
                            parameters["fee"] = fee_string;
                        });

                return std::make_optional(request);
            }
        };

        /**
        * Json Node type
        */
        typedef milecsa::transaction::Node<nlohmann::json> JsonNode;
    }
}

#endif //MILECSA_MILECSA_NODE_HPP
