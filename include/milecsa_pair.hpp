//
// Created by lotus mile on 03/10/2018.
//

#ifndef MILECSA_MILESCA_PAIR_HPP
#define MILECSA_MILESCA_PAIR_HPP

#pragma once

#include <string>
#include <optional>

#include "crypto.h"
#include "milecsa_result.hpp"
#include "milecsa_error.hpp"

namespace milecsa {

    namespace keys {

        class Pair;
        class Signer;
        class DigestCalculator;

        /**
         * Common keys protocol.
         * */
        class Key {
        public:

            /**
             * Clear key data
             */
            virtual void clear() = 0;

            /**
             * Encode key data to base58 string
             * @return encoded base58 string
             */
            virtual const std::string encode() const = 0;

            /**
             *
             * Decode key from base58 string
             *
             * @param key - encoded string
             * @param error - string error if key couldn't ber read from string
             * @return true or false
             */
            virtual bool decode(const std::string &key, std::string &error) = 0;
        };

        /**
         * Public key class.
         * */
        class PublicKey : protected ::PublicKey, public Key {

        public:

            void clear() override;

            const std::string encode() const override;

            bool decode(const std::string &key, std::string &error) override;

            friend class Pair;
            friend class Signer;
            friend class DigestCalculator;
        };

        /**
         * Private key class.
         * */
        class PrivateKey : protected ::PrivateKey, public Key {

        public:

            void clear() override;

            const std::string encode() const override;

            bool decode(const std::string &key, std::string &error) override;

            friend class Pair;
            friend class Signer;
            friend class DigestCalculator;
        };

        /**
         * Wallet pair.
         * */
        class Pair {

        public:

            /**
              * Create random keys pair.
              * @param error - error closure
              * @return optional wallet pair
              */
            static std::optional<Pair> Random(const ErrorHandler &error = default_error_handler);

            /**
             *  Create wallet pair from private key base58 encoded string.
             *
             * @param privateKey - private key encoded as base58 string
             * @param error - error closure
             * @return optional wallet pair
             */
            static std::optional<Pair> FromPrivateKey(
                    const std::string &privateKey,
                    const ErrorHandler &error = default_error_handler);

            /**
             *
             * Create wallet pair from secrete phrase.
             *
             * @param phrase - secret phrase
             * @param error - error closure
             * @return optional wallet pair
             */
            static std::optional<Pair> WithSecret(
                    const std::string &phrase,
                    const ErrorHandler &error = default_error_handler);

            /**
             * Validate wallet pair.
             *
             * @param pair - wallet pair
             * @param error - error closure
             * @return true if pair is valid
             */
            static bool Validate(const Pair &pair,
                                 const ErrorHandler &error = default_error_handler);


            /**
             * Validate wallet private key base58 encoded string.
             *
             * @param privateKey - private key encoded as base58 string
             * @param error - error closure
             * @return true if private key is valid
             */
            static bool ValidatePrivateKey(const std::string &privateKey,
                                           const ErrorHandler &error = default_error_handler);


            /**
             * Validate wallet public key base58 encoded string.
             *
             * @param publicKey - public key encoded as base58 string
             * @param error - error closure
             * @return  true if public key is valid
             */
            static bool ValidatePublicKey(const std::string &publicKey,
                                          const ErrorHandler &error = default_error_handler);

            /**
             * Public key getter.
             * */
            const PublicKey &get_public_key() const { return public_key_; };

            /**
             * Private key getter.
             * */
            const PrivateKey &get_private_key() const { return private_key_; };

            ~Pair() {
                clear();
            }

            friend class PublicKey;
            friend class PrivateKey;

        private:

            PublicKey public_key_;
            PrivateKey private_key_;

            bool validate(const ErrorHandler &error);

            void clear() {
                public_key_.clear();
                private_key_.clear();
            }

            static bool ValidateKey(Key &pairKey,
                                    const std::string &keyString,
                                    const std::string &name, const ErrorHandler &error);
        };

        class Signer : public ::Signer {
        public:
            Signer(const Pair &pair) : ::Signer(pair.get_private_key(), pair.get_public_key()) {}
        };

        class DigestCalculator: public ::DigestCalculator {
        public:

            void Update(const vector<unsigned char> &message) {
                ::DigestCalculator::Update(message);
            }

            void Update(const string &s, size_t size = 0) {
                ::DigestCalculator::Update(s, size);
            }

            void Update(const uint64_t &value, bool littleEndian = true) {
                ::DigestCalculator::Update(value, littleEndian);
            }

            void Update(unsigned int value, bool littleEndian = true) {
                ::DigestCalculator::Update(value, littleEndian);
            }

            void Update(const uint256_t &value, bool littleEndian = true) {
                ::DigestCalculator::Update(value, littleEndian);
            }

            void Update(unsigned short value, bool littleEndian = true){
                ::DigestCalculator::Update(value, littleEndian);
            }

            void Update(const PublicKey& pk) {
                ::DigestCalculator::Update(static_cast<const ::Array<ePublicKeySize>&>(pk));
            }

            void Update(const PrivateKey& pk) {
                ::DigestCalculator::Update(static_cast<const ::Array<ePrivateKeySize>&>(pk));
            }
        };

    }
}


#endif //MILECSA_MILESCA_PAIR_HPP
