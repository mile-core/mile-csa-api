#define BOOST_TEST_MODULE generate_pair

#include "milecsa_light_api.hpp"
#include "../MileTest.h"
#include <boost/test/included/unit_test.hpp>

struct Generator: public MileTest
{
    Generator():MileTest("Generator"){}

    bool generate() {
        milecsa::keys::Pair keyPair;

        if (milecsa::keys::generate(keyPair, errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        } else {
            print_key_pair(&keyPair);
            return true;
        }
    }

    bool generate_by_secret() {

        milecsa::keys::Pair keyPair;

        if (milecsa::keys::generate_with_secret(keyPair, "test string", errorDescription))
        {
            BOOST_TEST_MESSAGE("Error happened in milecsa_generate_keys_pair_with_secret");
            return false;
        }
        else
        {
           print_key_pair(&keyPair);
            return true;
        }
    }

    bool validate() {
        milecsa::keys::Pair initialKeyPair;

        if (milecsa::keys::generate(initialKeyPair, errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in Pair");
            return false;
        } else {
            milecsa::keys::Pair keyPair;

            if(milecsa::keys::generate_from_private_key(
                    keyPair,
                    initialKeyPair.get_private_key().encode(),
                    errorDescription)) {
                BOOST_TEST_MESSAGE("Error happened in generate_from_private_key");
                return false;
            }
            else
            {
                if(milecsa::keys::validate(keyPair,errorDescription)){
                    BOOST_TEST_MESSAGE("Error happened in validate");
                    return  false;
                }
                return true;
            }
        }

    }

    bool validate_public() {
        if(milecsa::keys::validate_public_key(
                "2HdKCuZ8xaYK9TX2Sewqni8NKnGDUL7TMsDJ9ApD9MJkSEocX1",
                errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in milecsa_validate_public_key");
            return false;
        }
        else
        {
            return true;
        }
    }

    bool validate_private() {

        if(milecsa::keys::validate_private_key(
                "TENDFTskF7RbYgLBLX6zusCVJMRGhU2aANktKXZfHAcXUJTAVisci8jAG79x8V2toc2ivpTahkDBvZGUeAnyewM8VfrFz",
                errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in milecsa_validate_private_key");
            return false;
        }
        else
        {
            return true;
        }
    }

    bool validate_public_fails() {
        if(milecsa::keys::validate_public_key("2000HdKCuZ8xaYK9TX2Sewqni8NKnGDUL7TMsDJ9ApD9MJkSE", errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in milecsa_validate_public_key");
            return false;
        }
        else
        {
            return true;
        }
    }

    bool validate_private_fails() {
        if(milecsa::keys::validate_private_key(
                "TENDFTskF7RbYgLBL00000002aANktKXZfHAcXUJTAVisci8jAG79x8V2toc2ivpTahkDBvZGUeAnyewM8VfrFz",
                errorDescription)) {
            BOOST_TEST_MESSAGE("Error happened in milecsa_validate_private_key");
            return false;
        }
        else
        {
            return true;
        }
    }
};

BOOST_FIXTURE_TEST_CASE( KeyPair, Generator )
{
    BOOST_CHECK(generate());
}

BOOST_FIXTURE_TEST_CASE( KeyPairBySecret, Generator )
{
    BOOST_CHECK(generate_by_secret());
}


BOOST_FIXTURE_TEST_CASE( ValidatePair, Generator )
{
    BOOST_CHECK(validate());
}

BOOST_FIXTURE_TEST_CASE( ValidatePublicKey, Generator )
{
    BOOST_CHECK(validate_public());
}

BOOST_FIXTURE_TEST_CASE( ValidatePrivateKey, Generator )
{
    BOOST_CHECK(validate_private());
}

BOOST_FIXTURE_TEST_CASE( ValidatePublicKeyFails, Generator )
{
    BOOST_CHECK(!validate_public_fails());
}

BOOST_FIXTURE_TEST_CASE( ValidatePrivateKeyFails, Generator )
{
    BOOST_CHECK(!validate_private_fails());
}


