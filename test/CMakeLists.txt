
set (MILE_TEST_LIB milecsa_test)

set (BOOST_COMPONENTS
        unit_test_framework
        )

include_directories(
        ./
        ../../include
        ../../vendor/crypto
        ../../vendor/nlohmann
)

find_package (Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})

add_subdirectory(initial)
add_subdirectory(milecsa_keys_pair_test)
add_subdirectory(milecsa_transactions_test)
enable_testing ()
