#pragma once
#include "../MileTest.h"

static auto error_handler = [](milecsa::result code, const std::string &error) mutable -> void {
    BOOST_TEST_MESSAGE("Error happened in MIleTestTransaction: " + error);
};

struct MIleTestTransaction: public MileTest  {

    MIleTestTransaction(const std::string &name) : MileTest(name) {
        keyPair = milecsa::keys::Pair::WithSecret("secret-phrase", error_handler);
        if (!keyPair)
            return ;
        else
        {
            print_key_pair(keyPair);
        }

        destination = milecsa::keys::Pair::WithSecret("destination-secret-phrase", error_handler);
        if (!destination)
            return ;
        else
        {
            print_key_pair(destination);
        }
    }

    std::optional<milecsa::keys::Pair> keyPair;
    std::optional<milecsa::keys::Pair> destination;
    const std::string nodeAddress = "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz";

};

