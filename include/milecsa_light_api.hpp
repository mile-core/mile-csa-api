#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string>

namespace milecsa {

    namespace light {
        struct Pair {
            std::string public_key;
            std::string private_key;
        };

        /**
          * Common mile csa handling codes
          *
          * */
        typedef enum {
            UNKNOWN = -1,
            OK = 0,
            FAIL = 1,
            EMPTY = 2,
            NOT_SUPPORTED = 3,
            NOT_FOUND = 4,
            ALREADY_EXIST = 5,
            EXCEPTION = 6,
            LAST = EXCEPTION
        } result;
    }

    namespace keys {

        /** Generate random Wallet Key Pair
         *
         * */
        light::result generate(
                light::Pair &keysPair,
                std::string &errorMessage);


        /** Generate Wallet pair from private key
         *
         * */
        light::result generate_from_private_key(
                light::Pair &keysPair,
                const std::string &privateKey,
                std::string &errorMessage);


        /**
         *
         * */
        light::result generate_with_secret(
                light::Pair &keysPair,
                const std::string &phrase,
                std::string &errorMessage);


        /** Validate key pair, (i.e. exist the pairs on curve ed25519 or not)
         *
         * */
        light::result validate(
                const light::Pair &keyPair,
                std::string &errorMessage);


        /**
         * Validate public key, (i.e. exist the pairs on curve ed25519 or not)
         *
         * */
        light::result validate_public_key(
                const std::string &publicKey,
                std::string &errorMessage);


        /**  Validate private key, (i.e. exist the pairs on curve ed25519 or not)
         *
         *
         * */
        light::result validate_private_key(
                const std::string &privateKey,
                std::string &errorMessage);


    }

    namespace transaction {

        static const uint64_t default_transaction = UINT_FAST64_MAX;

        /** Prepare signed transaction JSON packet for transfer assets to send to the MILE blockchain.
         *
         * */
        light::result prepare_transfer(const std::string &privateKey,
                                       const std::string &destinationPublicKey,

                                       const std::string &blockId,
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

        light::result prepare_emission(const std::string &privateKey,
                                       const std::string &dstWalletPublicKey,

                                       const std::string &blockId,
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
        light::result prepare_register_node(const std::string &privateKey,
                                            const std::string &nodeAddress,

                                            const std::string &blockId,
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
        light::result prepare_unregister_node(const std::string &privateKey,
                                              const std::string &nodeAddress,

                                              const std::string &blockId,
                                              const uint64_t  transactionId,


                                              std::string &transaction,
                                              std::string &digest,
                                              std::string &errorMessage);

        /** Prepare signed transaction as JSON packet to send to network vote for current asset rate.
         *
         *
         * */
        light::result prepare_vote_for_asstes_rate(const std::string &privateKey,

                                                   const std::string &blockId,
                                                   const uint64_t    transactionId,

                                                   unsigned short asset,
                                                   const std::string &rate,

                                                   std::string &transaction,
                                                   std::string &digest,
                                                   std::string &errorMessage);
    }
}

