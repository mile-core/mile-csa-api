#pragma once
#include "../MileTest.h"

struct MIleTestTransaction: public MileTest  {

    MIleTestTransaction(const std::string &name) : MileTest(name) {
        if (milecsa::keys::generate_with_secret(keyPair, "the most secrete phrase", errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in Pair");
        }
        else {
            print_key_pair(&keyPair);
        }
    }

    milecsa::light::Pair keyPair;
    const std::string nodeAddress = "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz";

};

