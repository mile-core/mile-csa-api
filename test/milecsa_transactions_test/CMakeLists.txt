
include_directories(
        ../
        ../../include
        ../../vendor/crypto
        ../../vendor/nlohmann
)

file (GLOB TESTS_SOURCES ${TESTS_SOURCES}
        ../MileTest.h
        *.cpp
        )

set (TEST transactions_test_${PROJECT_LIB})

add_executable(${TEST} ${TESTS_SOURCES})
target_link_libraries (${TEST} ${PROJECT_LIB} ${PROJECT_LIB} ${Boost_LIBRARIES})
add_test (NAME transactions COMMAND ${TEST})
enable_testing ()
