//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_VOTING_TEST_HPP
#define MILECSA_MILECSA_VOTING_TEST_HPP

#include "milecsa.hpp"
#include "MIleTestTransaction.hpp"

struct Vote: public MIleTestTransaction {

    Vote():MIleTestTransaction("Vote"){}

    bool test() {

        uint256_t bid   = 0;
        uint64_t trx_id =  (uint64_t)rand();

        if(auto emission = milecsa::transaction::Vote<nlohmann::json>::CreateRequest(
                *keyPair,
                bid,
                trx_id,
                milecsa::assets::XDR,
                1.1f,
                error_handler)){

            BOOST_TEST_MESSAGE("Wallet    trx: " + emission->get_body()->dump());
            BOOST_TEST_MESSAGE("Wallet digest: " + emission->get_digest().value_or("wrong digest..."));

            return true;
        }
        return  false;
    }
};

#endif //MILECSA_MILECSA_VOTING_TEST_HPP
