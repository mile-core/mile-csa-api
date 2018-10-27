cmake_minimum_required(VERSION 3.2)

set (CMAKE_CXX_STANDARD 17)

# Name of the project (will be the name of the plugin)
project(milecsa)

# Build a shared library named after the project from the files in `src/`

file(GLOB MILECSA_SOURCES
        ../../vendor/mile-crypto/src/*.cpp
        ../../vendor/mile-crypto/src/*.c
        ../../vendor/mile-crypto/src/ed25519/src/*.c
        ../../src/*.cpp
        src/*.cpp
        )

# Boost dependency

# Include BoostLib module
file(GLOB_RECURSE boostlib_cmake_path "${CMAKE_CURRENT_SOURCE_DIR}/node_modules" "BoostLib.cmake")
list(GET boostlib_cmake_path 0 boostlib_cmake_path)
get_filename_component(boostlib_cmake_path "${boostlib_cmake_path}" DIRECTORY)
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${boostlib_cmake_path}")
include(BoostLib)

# Locate/Download Boost (semver)
require_boost_libs("= 1.66.0" "system")

# Essential include files to build a node addon,
# you should add this line in every CMake.js based project.
include_directories(
        ./
        ../../include
        ../../vendor/mile-crypto/include
        ../../vendor/mile-crypto/src/ed25519/include
        ../../vendor/mile-crypto/src/private_include
        ../../vendor/nlohmann
        ${CMAKE_JS_INC}
        ${Boost_INCLUDE_DIRS}
)

# This line will tell CMake that we're building a shared library
# from the above source files
# named after the project's name
add_library(${PROJECT_NAME} SHARED ${MILECSA_SOURCES})

# This line will give our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.
# Define Boost dependencies there.
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB};${Boost_LIBRARIES})