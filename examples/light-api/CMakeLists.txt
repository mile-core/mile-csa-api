cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 11)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(PROJECT build-transaction-example)

find_package(milecsa CONFIG REQUIRED)

include_directories(
        ${milecsa_INCLUDE_PATH}
)

add_executable(${PROJECT} build-transaction.cpp)
target_link_libraries(${PROJECT} milecsa)