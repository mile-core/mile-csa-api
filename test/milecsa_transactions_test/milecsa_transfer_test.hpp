//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_TRANSFER_TEST_HPP
#define MILECSA_MILECSA_TRANSFER_TEST_HPP

#include "milecsa.hpp"
#include "MIleTestTransaction.hpp"
#include "json.hpp"

using json = nlohmann::json;
using transfer = milecsa::transaction::Transfer<json>;

struct Wallet: public MIleTestTransaction {

    Wallet():MIleTestTransaction("Wallet"){}

    bool simple_test() {

        auto pair = milecsa::keys::Pair::FromPrivateKey(keyPair->get_private_key().encode());

        auto t = transfer::CreateRequest(
                *pair,
                destination->get_public_key().encode(),
                0, // block id
                0, // trx id
                milecsa::assets::XDR, // asset code
                10);

        BOOST_TEST_MESSAGE("Simple transaction class name: " + std::string(typeid(t).name()));

        if (auto trx_body = transfer::CreateRequest(
                *pair,
                destination->get_public_key().encode(),
                0, // block id
                0, // trx id
                milecsa::assets::XDR, // asset code
                1000)->get_body()){
            BOOST_TEST_MESSAGE("Simple transaction : " + trx_body->dump());
            return true;
        }
        else {
            return false;
        }
    }

    bool test() {

        uint256_t bid   = 0;
        uint64_t trx_id =  (uint64_t)rand();

        if (auto transfer = milecsa::transaction::Transfer<json>::CreateRequest(
            *keyPair,
            destination->get_public_key().encode(),
            bid,
            trx_id,
            milecsa::assets::XDR,
            1.1f,
            0.0f,
            "memo",
            error_handler)) {

            BOOST_TEST_MESSAGE("Wallet    trx: " + transfer->get_body()->dump());
            BOOST_TEST_MESSAGE("Wallet digest: " + transfer->get_digest().value_or("wrong digest..."));

            return true;
        }
        return false;
    }
};

#endif //MILECSA_MILECSA_TRANSFER_TEST_HPP
