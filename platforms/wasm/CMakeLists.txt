set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_SYSTEM_NAME Emscripten)

# Help CMake find the platform file
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/../modules ${EMSCRIPTEN_ROOT})

if(NOT EMSCRIPTEN_PREFIX)
    if(DEFINED ENV{EMSCRIPTEN})
        set(EMSCRIPTEN_PREFIX $ENV{EMSCRIPTEN})
    else()
        set(EMSCRIPTEN_PREFIX "/usr/local/Cellar/emscripten/1.38.12/libexec")
    endif()
endif()

set(EMSCRIPTEN_TOOLCHAIN_PATH "${EMSCRIPTEN_PREFIX}/system")

if(CMAKE_HOST_WIN32)
    set(EMCC_SUFFIX ".bat")
else()
    set(EMCC_SUFFIX "")
endif()
set(CMAKE_C_COMPILER "${EMSCRIPTEN_PREFIX}/emcc${EMCC_SUFFIX}")
set(CMAKE_CXX_COMPILER "${EMSCRIPTEN_PREFIX}/em++${EMCC_SUFFIX}")
set(CMAKE_AR "${EMSCRIPTEN_PREFIX}/emar${EMCC_SUFFIX}" CACHE FILEPATH "Emscripten ar")
set(CMAKE_RANLIB "${EMSCRIPTEN_PREFIX}/emranlib${EMCC_SUFFIX}" CACHE FILEPATH "Emscripten ranlib")

set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH}
        "${EMSCRIPTEN_TOOLCHAIN_PATH}"
        "${EMSCRIPTEN_PREFIX}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Compared to the classic (asm.js) compilation, -s WASM=1 is added to both
# compiler and linker. The *_INIT variables are available since CMake 3.7, so
# it won't work in earlier versions. Sorry.

set(CMAKE_CXX_FLAGS_INIT "-s WASM=1")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-s WASM=1")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-DNDEBUG -O3")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT "-O3 --llvm-lto 1")

set (WASM ${PROJECT_LIB}.js)

include_directories (
        ../wrapper
        ../../include
        ../../vendor/mile-crypto/include
        ../../vendor/mile-crypto/src/private_include
        ../../vendor/mile-crypto/src/ed25519/include
        ../../vendor/nlohmann
)

FILE(GLOB WASM_SOURCES
        ../wrapper/*.cpp
        ../../vendor/mile-crypto/src/*.c
        ../../vendor/mile-crypto/src/*.cpp
        ../../vendor/mile-crypto/src/ed25519/src/*.c
        ../../src/*.cpp
        ./*.cpp
        )

add_executable(${WASM} ${WASM_SOURCES})

set_target_properties(${WASM} PROPERTIES LINK_FLAGS "-O3 -Oz --llvm-lto 1 -std=c++1z -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -s WASM=1 -s RESERVED_FUNCTION_POINTERS=1 --bind ")

target_link_libraries (${WASM}  ${Boost_LIBRARIES})

