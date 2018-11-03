cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)

set (BOOST_COMPONENTS)
find_package (Boost  REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(PROJECT send-1xdr)

find_package(milecsa CONFIG REQUIRED)
find_package(CURL)

include_directories(
        ${milecsa_INCLUDE_PATH}
)

if (${CURL_FOUND})
    message(STATUS "curl library version: " ${CURL_VERSION_STRING})
    #add_compile_definitions(WITH_CURL)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWITH_CURL")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_CURL")
endif ()

add_executable(${PROJECT} send-1xdr.cpp)
target_link_libraries(
        ${PROJECT}
        milecsa
        curl
        )