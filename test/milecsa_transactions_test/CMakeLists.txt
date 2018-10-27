
include_directories(
        ../
        ../../include
        ../../vendor/nlohmann
)

file (GLOB TESTS_SOURCES ${TESTS_SOURCES}
        ../MileTest.h
        *.cpp
        )

set (TEST transactions_test_${PROJECT_LIB})

find_package(CURL)

add_executable(${TEST} ${TESTS_SOURCES})

set(EXT_LIBS ${TEST} ${PROJECT_LIB} ${PROJECT_LIB} ${Boost_LIBRARIES})

if (${CURL_FOUND})
    message(STATUS "curl library version: " ${CURL_VERSION_STRING})
    add_compile_definitions(WITH_CURL)
    target_link_libraries (${EXT_LIBS} curl)
else()
    target_link_libraries (${EXT_LIBS})
endif ()

target_link_libraries (${TEST} ${PROJECT_LIB} ${PROJECT_LIB} ${Boost_LIBRARIES})
add_test (NAME transactions COMMAND ${TEST})
enable_testing ()
