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

        uint256_t bid   = get_block_id();

        srand(time(0));
        uint64_t trx_id =  (uint64_t)rand();

        if (auto transfer = milecsa::transaction::Emission<json>::CreateRequest(
                *keyPair,
                bid,
                trx_id,
                milecsa::assets::MILE,
                0.0f,
                error_handler)) {

            auto trx_body = transfer->get_body()->dump();

            BOOST_TEST_MESSAGE("Wallet    trx: " + trx_body);
            BOOST_TEST_MESSAGE("Wallet digest: " + transfer->get_digest().value_or("wrong digest..."));

            return send(trx_body) == 0;

        }
        return false;

    }
};

#endif //MILECSA_MILECSA_EMISSION_TEST_HPP
