#define BOOST_TEST_MODULE initial

#include "milecsa.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Equal )
{

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

    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(0.00001f), "0.00001");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(1.00001f), "1.00001");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(1.99999f), "1.99999");
    BOOST_CHECK_EQUAL(milecsa::assets::MILE.value_to_string(1.999999f),"1.99999");
}

