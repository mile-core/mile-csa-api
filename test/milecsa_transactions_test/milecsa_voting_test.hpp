//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_VOTING_TEST_HPP
#define MILECSA_MILECSA_VOTING_TEST_HPP

#include "milecsa_light_api.hpp"
#include "MIleTestTransaction.hpp"

struct Vote: public MIleTestTransaction {

    Vote():MIleTestTransaction("Vote"){}

    bool test() {

        std::string transaction;
        std::string digest;

        if (milecsa::transaction::prepare_vote_for_asstes_rate(
                keyPair,
                0,
                0,
                0,
                "10.0",
                transaction,
                digest,
                errorDescription)){
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        }

        BOOST_TEST_MESSAGE("Wallet    trx: " + transaction);
        BOOST_TEST_MESSAGE("Wallet digest: " + digest);

        return  true;

    }
};

#endif //MILECSA_MILECSA_VOTING_TEST_HPP
