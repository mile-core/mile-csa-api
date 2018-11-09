#define BOOST_TEST_MODULE generate_pair

#include "../MileTest.h"
#include <boost/test/included/unit_test.hpp>

static auto error_handler = [](milecsa::result code, const std::string &error) mutable -> void {
    BOOST_TEST_MESSAGE("Error happened in Generator: " + error);
};

struct Generator: public MileTest
{
    Generator():MileTest("Generator"){}

    bool generate() {
        auto keyPair = milecsa::keys::Pair::Random(error_handler);

        if (!keyPair) {
            return false;
        } else {
            print_key_pair(keyPair);
            return true;
        }
    }

    bool generate_by_secret() {

        auto keyPair = milecsa::keys::Pair::WithSecret("secret-phrase", error_handler);

        if (!keyPair)
        {
            return false;
        }
        else
        {
            print_key_pair(keyPair);
            return true;
        }
    }

    bool validate() {
        auto initialKeyPair = milecsa::keys::Pair::Random(error_handler);

        if (!initialKeyPair) {
            return false;
        } else {
            std::optional<milecsa::keys::Pair> keyPair = milecsa::keys::Pair::FromPrivateKey(initialKeyPair->get_private_key().encode(), error_handler);

            if(!keyPair) {
                return false;
            }
            else
            {
                if(!milecsa::keys::Pair::Validate(*keyPair, error_handler)){
                    return  false;
                }
            }
            return true;
        }

    }

    bool validate_public(const std::string &pk = "2HdKCuZ8xaYK9TX2Sewqni8NKnGDUL7TMsDJ9ApD9MJkSEocX1") {
        if(!milecsa::keys::Pair::ValidatePublicKey(pk, error_handler)) {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool validate_private(const std::string &pk = "TENDFTskF7RbYgLBLX6zusCVJMRGhU2aANktKXZfHAcXUJTAVisci8jAG79x8V2toc2ivpTahkDBvZGUeAnyewM8VfrFz") {

        if(!milecsa::keys::Pair::ValidatePrivateKey(pk, error_handler)) {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool validate_public_fails() {
        return  validate_public("2000HdKCuZ8xaYK9TX2Sewqni8NKnGDUL7TMsDJ9ApD9MJkSE");

    }

    bool validate_private_fails() {
        return validate_private("TENDFTskF7RbYgLBL00000002aANktKXZfHAcXUJTAVisci8jAG79x8V2toc2ivpTahkDBvZGUeAnyewM8VfrFz");
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


