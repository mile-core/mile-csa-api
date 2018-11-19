//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_TRANSFER_TEST_HPP
#define MILECSA_MILECSA_TRANSFER_TEST_HPP

#include "milecsa.hpp"
#include "MIleTestTransaction.hpp"
#include "json.hpp"
#include <time.h>

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
                0.1);

        BOOST_TEST_MESSAGE("Simple transaction class name: " + std::string(typeid(t).name()));

        if (auto trx_body = transfer::CreateRequest(
                *pair,
                destination->get_public_key().encode(),
                0, // block id
                0, // trx id
                milecsa::assets::XDR, // asset code
                0.1,
                0,
                "Y29uc3QgcGF0aCA9IHJlcXVpcmUoJ3BhdGgnKTsKCm1vZHVsZS5leHBvcnRzID0gewogIGRldlNlcnZlcjogewogICAgcHJveHk6ICdodHRwczovL2V4cGxvcmVyLnRlc3RuZXQubWlsZS5nbG9iYWwnLAogICAgLy9wcm94eTogJ2h0dHA6Ly9sb2NhbGh")->get_body()){
            BOOST_TEST_MESSAGE("Simple transaction : " + trx_body->dump());
            return true;
        }
        else {
            return false;
        }
    }

    bool test() {

        uint256_t bid   = get_block_id();

        srand(time(0));

        uint64_t trx_id =  (uint64_t)rand();

        if (auto transfer = milecsa::transaction::Transfer<json>::CreateRequest(
            *keyPair,
            destination->get_public_key().encode(),
            bid,
            trx_id,
            milecsa::assets::XDR,
            0.1f,
            0.0f,
            "memo",
            error_handler)) {

            auto trx_body = transfer->get_body()->dump();

            BOOST_TEST_MESSAGE("Wallet    trx: " + trx_body);
            BOOST_TEST_MESSAGE("Wallet digest: " + transfer->get_digest().value_or("wrong digest..."));

            return send(trx_body) == 0;
        }
        return false;
    }
};

#endif //MILECSA_MILECSA_TRANSFER_TEST_HPP
