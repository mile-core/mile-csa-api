#define BOOST_TEST_MODULE initial

#include "milecsa.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Equal )
{

    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.31f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.301f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(0.1f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.1f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.01f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.001f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.99f) << std::endl;
    std::cerr << "XDR " << milecsa::assets::XDR.value_to_string(1.999f) << std::endl;

    std::cerr << "MILE " << milecsa::assets::MILE.value_to_string(0.00001f) << std::endl;
    std::cerr << "MILE " << milecsa::assets::MILE.value_to_string(0.000001f) << std::endl;
    std::cerr << "MILE " << milecsa::assets::MILE.value_to_string(1.000001f) << std::endl;
    std::cerr << "MILE " << milecsa::assets::MILE.value_to_string(9.99999f) << std::endl;
    std::cerr << "MILE " << milecsa::assets::MILE.value_to_string(9.999999f) << std::endl;

    std::cerr << "NILL " << milecsa::assets::NILL.value_to_string(0.00001f) << std::endl;
    std::cerr << "NILL " << milecsa::assets::NILL.value_to_string(1000000.00001f) << std::endl;

    std::cerr << "Token from code 0: " << milecsa::assets::TokenFromCode(0).name << std::endl;
    std::cerr << "Token from code 1: " << milecsa::assets::TokenFromCode(1).name << std::endl;
    std::cerr << "Token from code 2: " << milecsa::assets::TokenFromCode(2).name << std::endl;

    BOOST_CHECK_EQUAL(milecsa::assets::TokenFromCode(milecsa::assets::XDR.code).code, milecsa::assets::XDR.code);
    BOOST_CHECK_EQUAL(milecsa::assets::TokenFromCode(milecsa::assets::MILE.code).code, milecsa::assets::MILE.code);
    BOOST_CHECK_EQUAL(milecsa::assets::TokenFromCode(10).code, milecsa::assets::NILL.code);
    BOOST_CHECK_EQUAL(milecsa::assets::XDR.value_to_string(1.10001f), "1.10");
    BOOST_CHECK_EQUAL(milecsa::assets::XDR.value_to_string(1.10901f), "1.10");
    BOOST_CHECK_EQUAL(milecsa::assets::XDR.value_to_string(1.99901f), "1.99");

    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(0.00001f), "0.00001");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(1.00001f), "1.00001");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(1.99999f), "1.99999");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(1.999999f),"1.99999");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(0.1f+0.2f),"0.30000");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(.29f*100.0f),"29.00000");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(0.000011f*10.0f),"0.00011");
}

