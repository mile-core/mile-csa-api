
include_directories(
        ../
        ../../include
        ../../vendor/crypto
)

file (GLOB TESTS_SOURCES ${TESTS_SOURCES}
        ../MileTest.h
        *.cpp
        )

set (TEST keys_pair_test_${PROJECT_LIB})

add_executable(${TEST} ${TESTS_SOURCES})
target_link_libraries (${TEST} ${PROJECT_LIB} ${PROJECT_LIB} ${Boost_LIBRARIES})
add_test (NAME keyspair COMMAND ${TEST})
enable_testing ()
