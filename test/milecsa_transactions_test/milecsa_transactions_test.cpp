#define BOOST_TEST_MODULE transactions

#include "../MileTest.h"
#include "milecsa_transfer_test.hpp"
#include "milecsa_nodes_test.hpp"
#include "milecsa_emission_test.hpp"
#include "milecsa_voting_test.hpp"

BOOST_FIXTURE_TEST_CASE( TransferSimple, Wallet )
{
    BOOST_CHECK(simple_test());
}

BOOST_FIXTURE_TEST_CASE( TransferAsset, Wallet )
{
    BOOST_CHECK(test());
}

BOOST_FIXTURE_TEST_CASE( RegisterNode, Node )
{
    BOOST_CHECK(register_node());
    BOOST_CHECK(!register_node("some invalide address name"));
    BOOST_CHECK(!register_node("localhost"));
}

BOOST_FIXTURE_TEST_CASE( UnregisterNode, Node )
{
    BOOST_CHECK(unregister_node());
}

BOOST_FIXTURE_TEST_CASE( EmissionAsset, Emission )
{
    BOOST_CHECK(test());
}

BOOST_FIXTURE_TEST_CASE( VotingAssetsRate, Vote )
{
    BOOST_CHECK(test());
}



