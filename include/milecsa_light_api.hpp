#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <optional>
#include <functional>
#include "crypto_types.h"
#include "milecsa_result.hpp"
#include "milecsa_pair.hpp"

namespace milecsa {

    namespace keys {

        /** Generate random Wallet Key Pair
         *
         * */
        result generate(
                Pair &keysPair,
                std::string &errorMessage);


        /** Generate Wallet pair from private key
         *
         * */
        result generate_from_private_key(
                Pair &keysPair,
                const std::string &privateKey,
                std::string &errorMessage);


        /**
         *
         * */
        result generate_with_secret(
                Pair &keysPair,
                const std::string &phrase,
                std::string &errorMessage);


        /** Validate key pair, (i.e. exist the pairs on curve ed25519 or not)
         *
         * */
        result validate(
                const Pair &keyPair,
                std::string &errorMessage);


        /**
         * Validate public key, (i.e. exist the pairs on curve ed25519 or not)
         *
         * */
        result validate_public_key(
                const std::string &publicKey,
                std::string &errorMessage);


        /**  Validate private key, (i.e. exist the pairs on curve ed25519 or not)
         *
         *
         * */
        result validate_private_key(
                const std::string &privateKey,
                std::string &errorMessage);


    }

    namespace transaction {

        /** Prepare signed transaction JSON packet for transfer assets to send to the MILE blockchain.
         *
         * */
        result prepare_transfer(const milecsa::keys::Pair &keyPair,
                                const std::string &dstWalletPublicKey,

                                const uint256_t blockId,
                                const uint64_t  transactionId,
                                unsigned short asset,
                                const std::string &amount,
                                const std::string &description,
                                const std::string &fee,

                //
                // Signed json
                //
                                std::string &transaction,
                                std::string &digest,
                                std::string &errorMessage);

        result prepare_emission(const milecsa::keys::Pair &keyPair,
                                const std::string &dstWalletPublicKey,

                                const uint256_t blockId,
                                const uint64_t  transactionId,
                                unsigned short asset,
                                const std::string &amount,
                                const std::string &description,
                                const std::string &fee,

                //
                // Signed json
                //
                                std::string &transaction,
                                std::string &digest,
                                std::string &errorMessage);

        /** Prepare signed node transaction as JSON packet to send to network to register the node has nodeAddress.
         *
         *
         *
         * */
        result prepare_register_node(const milecsa::keys::Pair &keyPair,
                                     const std::string &nodeAddress,

                                     const uint256_t blockId,
                                     const uint64_t  transactionId,
                                     unsigned short asset,
                                     const std::string &amount,

                //
                // Signed json
                //
                                     std::string &transaction,
                                     std::string &digest,
                                     std::string &errorMessage);



        /** Prepare signed transaction as JSON packet to send to network to dismiss a node associated the keys pair.
         *
         *
         * */
        result prepare_unregister_node(const milecsa::keys::Pair &keyPair,
                                       const std::string &nodeAddress,

                                       const uint256_t blockId,
                                       const uint64_t  transactionId,


                                       std::string &transaction,
                                       std::string &digest,
                                       std::string &errorMessage);

        /** Prepare signed transaction as JSON packet to send to network vote for current asset rate.
         *
         *
         * */
        result prepare_vote_for_asstes_rate(const milecsa::keys::Pair &keyPair,

                                            const uint256_t blockId,
                                            const uint64_t  transactionId,

                                            unsigned short asset,
                                            const std::string &rate,

                                            std::string &transaction,
                                            std::string &digest,
                                            std::string &errorMessage);
    }
}

