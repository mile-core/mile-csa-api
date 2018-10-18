include_directories (
        ../include
        ../vendor/crypto
        ../vendor/ed25519/src
        ../vendor/nlohmann/
)

list(APPEND SOURCES

        ../vendor/ed25519/src/add_scalar.c
        ../vendor/ed25519/src/fe.c
        ../vendor/ed25519/src/ge.c
        ../vendor/ed25519/src/key_exchange.c
        ../vendor/ed25519/src/keypair.c
        ../vendor/ed25519/src/sc.c
        ../vendor/ed25519/src/seed.c
        ../vendor/ed25519/src/sha512.c
        ../vendor/ed25519/src/sign.c
        ../vendor/ed25519/src/verify.c
        ../vendor/crypto/base58.cpp
        ../vendor/crypto/crypto.cpp
        ../vendor/crypto/ed25519_ext.cpp
        ../vendor/crypto/sha3.c

        ../src/milecsa_crypto_types.cpp
        ../src/milecsa_pair.cpp
        ../src/milecsa_pair_wrapper.cpp
        ../src/milecsa_utils.cpp
        ../src/transactions_wrapper/milecsa_emission.cpp
        ../src/transactions_wrapper/milecsa_nodes.cpp
        ../src/transactions_wrapper/milecsa_transfer.cpp
        ../src/transactions_wrapper/milecsa_voting.cpp
)


set (BOOST_COMPONENTS system)

find_package (Boost  REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})

if (MSVC)
    add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-Wall -pedantic)
else ()
    message ("Unknown compiler")
endif ()

add_library(${PROJECT_LIB}  ${SOURCES})

target_link_libraries (
        ${PROJECT_LIB}  PUBLIC
        ${Boost_LIBRARIES}
)

target_include_directories(
        ${PROJECT_LIB}
        PUBLIC
        ../vendor/nlohmann
        ../include
        ../vendor/crypto
)

FILE(GLOB INCLUDE_FILES
        ../vendor/nlohmann/*.hpp
        ../vendor/crypto/*.h
        ../vendor/crypto/*.hpp
        ../vendor/ed25519/src/*.h
        ../include/*.hpp
        )

install(FILES ${INCLUDE_FILES} DESTINATION include)
install(TARGETS ${PROJECT_LIB} DESTINATION lib)
