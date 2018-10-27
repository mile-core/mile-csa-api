#pragma once

#include <optional>
#include <boost/test/included/unit_test.hpp>

#include "milecsa.hpp"

struct MileTest {

    MileTest(const std::string &name): name(name) {
        BOOST_TEST_MESSAGE("Setup "+name);
    }

    void print_key_pair(const std::optional<milecsa::keys::Pair> &keyPair) {
        std::string mess = "Public key: ";
        mess += keyPair->get_public_key().encode();
        BOOST_TEST_MESSAGE(mess);
        mess = "Private key: ";
        mess += keyPair->get_private_key().encode();
        BOOST_TEST_MESSAGE(mess);
    }

    ~MileTest(){
            if (!errorDescription.empty()) {
                    BOOST_TEST_MESSAGE(errorDescription);
            }
        BOOST_TEST_MESSAGE("Teardown "+name);
    }

    std::string errorDescription;
    std::string name;
};
