cmake_minimum_required(VERSION 3.0)

set(PROJECT_LIB milecsa)

set (CMAKE_CXX_STANDARD 17)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set (BOOST_COMPONENTS)
find_package (Boost  REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR} )
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    message(STATUS "Using Clang" )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    message(STATUS "Using gcc" )
    set(CMAKE_C_FLAGS "-fPIC")
    set(CMAKE_CXX_FLAGS "-fPIC -std=c++17")

    if (${CMAKE_SYSTEM_NAME} STREQUAL Linux)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DX86_LINUX")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")

    message( FATAL_ERROR "The project does not support Intel compiler" )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

    message( FATAL_ERROR "The project does not support MSVC compiler" )

endif()

option(wasm "wasm" OFF)


if(wasm)
    add_subdirectory(platforms/wasm)
else()
    add_subdirectory(lib)
    add_subdirectory(test)
    enable_testing ()
endif()
unset(platforms/wasm CACHE)

if (nodejs)
    add_custom_target (
            nodejs
            COMMAND npm install --prefix ../platforms/nodejs
    )
    add_custom_target (
            nodejs-test
            COMMAND npm test --prefix ../platforms/nodejs
    )
endif ()