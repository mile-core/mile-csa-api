//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_EMISSION_TEST_HPP
#define MILECSA_MILECSA_EMISSION_TEST_HPP

#include "milecsa.hpp"
#include "MIleTestTransaction.hpp"

using json = nlohmann::json;

struct Emission: public MIleTestTransaction {

    Emission():MIleTestTransaction("Emission"){}

    bool test() {

        uint256_t bid   = 0;
        uint64_t trx_id =  (uint64_t)rand();

        if (auto transfer = milecsa::transaction::Emission<json>::CreateRequest(
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

#endif //MILECSA_MILECSA_EMISSION_TEST_HPP
