//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_TRANSFER_TEST_HPP
#define MILECSA_MILECSA_TRANSFER_TEST_HPP

#include "milecsa.hpp"
#include "milecsa_light_api.hpp"
#include "MIleTestTransaction.hpp"
#include "json.hpp"

using json = nlohmann::json ;
using transfer = milecsa::transaction::Transfer<json>;

struct Wallet: public MIleTestTransaction {

    Wallet():MIleTestTransaction("Wallet"){}

    const std::string destination = "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz";

    bool simple_test() {

        auto pair = milecsa::keys::Pair::FromPrivateKey(keyPair.private_key);

        auto t = transfer::CreateRequest(
                *pair,
                destination,
                0, // block id
                0, // trx id
                0, // asset code
                "10");

        BOOST_TEST_MESSAGE("Simple transaction class name: " + std::string(typeid(t).name()));

        if (auto trx_body = transfer::CreateRequest(
                *pair,
                destination,
                0, // block id
                0, // trx id
                0, // asset code
                "1000")->get_body()){
            BOOST_TEST_MESSAGE("Simple transaction : " + trx_body->dump());
            return true;
        }
        else {
            return false;
        }
    }

    bool test() {

        std::string transaction;
        std::string fee;
        std::string digest;

        if (milecsa::transaction::prepare_transfer(
                keyPair.private_key,
                destination,
                "0",
                0,
                1,
                "1000",
                "memo",
                fee,

                transaction,
                digest,

                errorDescription)){
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet    fee: " + fee);
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return  true;

    }
};

#endif //MILECSA_MILECSA_TRANSFER_TEST_HPP
