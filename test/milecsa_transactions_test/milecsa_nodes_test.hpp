//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_NODES_TEST_HPP
#define MILECSA_MILECSA_NODES_TEST_HPP

#include "milecsa.hpp"
#include "MIleTestTransaction.hpp"
#include "json.hpp"

using json = nlohmann::json;

struct Node: public MIleTestTransaction {

    Node() : MIleTestTransaction("Node") {}

    bool register_node(const std::string &nodeAddress = "172.20.20.1") {

        if (auto node = milecsa::transaction::Node<json>::CreateRegisterRequest(
                *keyPair,
                nodeAddress,
                0,
                0,
                1000,
                0,
                error_handler)) {
            BOOST_TEST_MESSAGE("Wallet    trx: " + node->get_body()->dump());
            BOOST_TEST_MESSAGE("Wallet digest: " + node->get_digest().value_or("wrong digest..."));
            return true;
        }

        return false;
    }

    bool unregister_node() {
        std::string transaction;
        std::string digest;

        if(auto node = milecsa::transaction::Node<json>::CreateUnregisterRequest(
                *keyPair,
                0,
                0,
                0,
                error_handler)){

            BOOST_TEST_MESSAGE("Wallet    trx: " + node->get_body()->dump());
            BOOST_TEST_MESSAGE("Wallet digest: " + node->get_digest().value_or("wrong digest..."));

            return true;
        }
        return false;
    };
};

#endif //MILECSA_MILECSA_NODES_TEST_HPP
