//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_NODES_TEST_HPP
#define MILECSA_MILECSA_NODES_TEST_HPP

#include "milecsa_light_api.hpp"
#include "MIleTestTransaction.hpp"

struct Node: public MIleTestTransaction {

    Node() : MIleTestTransaction("Node") {}

    bool register_node(const std::string &nodeAddress = "172.20.20.1") {

        std::string transaction;
        std::string digest;

        if (milecsa::transaction::prepare_register_node(
                keyPair.private_key,
                nodeAddress,
                "0",
                0,
                0,
                "1000",
                transaction,
                digest,
                errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return true;
    }

    bool unregister_node() {
        std::string transaction;
        std::string digest;

        if(milecsa::transaction::prepare_unregister_node(
                keyPair.private_key,
                nodeAddress,
                "0",
                0,
                transaction,
                digest,
                errorDescription)){

            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return true;
    };
};

#endif //MILECSA_MILECSA_NODES_TEST_HPP
