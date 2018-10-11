//
// Created by lotus mile on 02/10/2018.
//

#ifndef MILECSA_MILECSA_EMISSION_TEST_HPP
#define MILECSA_MILECSA_EMISSION_TEST_HPP

#include "milecsa_light_api.hpp"
#include "MIleTestTransaction.hpp"

struct Emission: public MIleTestTransaction {

    Emission():MIleTestTransaction("Emission"){}

    bool test() {

        std::string transaction;
        std::string fee;
        std::string digest;

        if (milecsa::transaction::prepare_emission(
                keyPair,
                "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz",
                0,
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

#endif //MILECSA_MILECSA_EMISSION_TEST_HPP
