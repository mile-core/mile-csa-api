# Mile CSA C++ API

## Requirements
1. c++17
1. cmake
1. boost multiprecision installed includes (>=1.66, exclude 1.68!)
1. [Wasm compiler](https://webassembly.org): [emscripten](http://kripken.github.io/emscripten-site/)
1. [cmake-js](https://github.com/cmake-js/cmake-js) to build node.js addons


## Build
    $ git clone https://github.com/mile-core/mile-csa-api
    $ cd ./mile-csa-api; mkdir build; cd ./build
    $ cmake ..; make -j4
    $ make test

## Build WASM extension to use the API in a javascript code
    $ cmake -Dwasm=True ..; make -j4; cd platforms/wasm
    $ cp ../../../platforms/wasm/milecsa_wasm_test.html ./
    $ emrun --serve_root ./ --browser chrome milecsa_wasm_test.html

or in case of using Node.js install `http-server` once

    $ npm install http-server -g

and then

    $ cmake -Dwasm=True ..; make -j4; cd platforms/wasm
    $ cp ../../../platforms/wasm/milecsa_wasm_test.html ./index.html
    $ http-server -o


## Build NODE.JS Addon to use the API in a node.js 
Install cmake-js if it needs:

    $ npm install -g cmake-js
    
Build addon milecsa with boost module on **Ubuntu18.04**:

    $ cd platforms/nodejs
    $ npm install
    $ cmake-js rebuild
    $ npm test

Build addon milecsa mudule on **mac os**:
  
    $ mkdir -p build; cd build;
    $ cmake -Dnodejs=True ..; make nodejs;
    $ make nodejs-test
          

## Update Ubuntu18.04 nodejs to v10
    $ sudo apt install curl
    $ curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
    $ sudo apt install nodejs
    $ node --version

## How to use javascript MILE CSA API
* [MILE CSA JavaScript API](https://github.com/mile-core/mile-csa-api/blob/master/platforms/wasm/Readme.md)
* emrun --serve_root ./ --browser chrome your_app.html

## Boost updates (if it needs)
    $ wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
    $ tar -xzf boost_1_*
    $ cd boost_1_*
    $ ./bootstrap.sh --prefix=/usr
    $ ./b2 install --prefix=/usr --with=all -j4


## Tested
1. Centos7 (gcc v7.0)
1. Ubuntu 18.4
1. OSX 10.13, XCode10

### Wallet public/private keys Pair class

```cpp
    #include "milecsa.hpp"

    // to create a wallet pair you only use one of the follow methodes
    //
    //  milecsa::keys::Pair::Random
    //  milecsa::keys::Pair::FromPrivateKey
    //  milecsa::keys::Pair::WithSecret

    auto pair = milecsa::keys::Pair::Random(...);

    //
    //
    // getters
    //

    milecsa::keys::Key public_key  = pair.get_public_key();
    milecsa::keys::Key private_key = pair.get_private_key();

    std::cout << " Public key as base58 encoded string : " << public_key.encode() << std::endl;
    std::cout << " Private key as base58 encoded string: " << private_key.encode() << std::endl;

```

### Make new wallet

```cpp
    #include "milecsa.hpp"
    if (auto pair =
            milecsa::keys::Pair::Random([errorMessage, &result](
                    milecsa::result code,
                    std::string error) mutable -> void {

                //
                // Handle error. For example, the block can contain async error processing.
                //

            }))
    {
        //
        // Handle wallet pair.
        //

        std::cout << " Public key: " << pair->get_public_key().encode() << std::endl;
        std::cout << " Private key: " << pair->get_private_key().encode() << std::endl;
    }
```

### Restore wallet pair from private key encoded as base58 string

```cpp
    #include "milecsa.hpp"

    std::strint pk = "TENDFTskF7RbYgLBLX6zusCVJMRGhU2aANktKXZfHAcXUJTAVisci8jAG79x8V2toc2ivpTahkDBvZGUeAnyewM8VfrFz";

    if (auto pair =
             milecsa::keys::Pair::FromPrivateKey(pk, [errorMessage, &result](
                     milecsa::result code,
                     std::string error) mutable -> void {
                 result = code;
                 errorMessage = error;
             }))
     {
             //
             // Handle error.
             //
     }
     else {
        //
        // Handle wallet pair.
        //

        std::cout << " Public key: " << pair->get_public_key().encode() << std::endl;
        std::cout << " Private key: " << pair->get_private_key().encode() << std::endl;
     }
```

### Make new wallet from secrete phrase

```cpp
    #include "milecsa.hpp"

    std::string secret = "the most secret phrase";

    if (auto pair =
            milecsa::keys::Pair::WithSecret(secret, [errorMessage, &result](
                    milecsa::result code,
                    std::string error) mutable -> void {

                //
                // Handle error. For example, the block can contain async error processing.
                //

            }))
    {
        //
        // Handle wallet pair.
        //

        std::cout << " Public key: " << pair->get_public_key().encode() << std::endl;
        std::cout << " Private key: " << pair->get_private_key().encode() << std::endl;
    }
```

### Validate wallet keys

#### Validate private key encoded as base58 string
```cpp
    #include "milecsa.hpp"

    std::strint pk = "TENDFTskF7RbYgLBLX6zusCVJMRGhU2aANktKXZfHAcXUJTAVisci8jAG79x8V2toc2ivpTahkDBvZGUeAnyewM8VfrFz";

    if (milecsa::keys::Pair::ValidatePrivateKey(pk, [errorMessage, &result](
            milecsa::result code,
            std::string error) mutable -> void {

            //
            // Handling invalid private key
            //

    })) {
        //
        // Handling valid key
        //
    }
```

#### Validate public key encoded as base58 string
```cpp
    #include "milecsa.hpp"

    std::strint pk = "7PNJQMVBwUccsQsmGh93KQYget74UXyoHUDpUbARLuUw6F53o";

    if (milecsa::keys::Pair::ValidatePublicKey(pk, [errorMessage, &result](
            milecsa::result code,
            std::string error) mutable -> void {

            //
            // Handling invalid private key
            //

    })) {
        //
        // Handling valid key
        //
    }
```

### Preparing wallet transactions request

#### Asset transfer request. Simple getting transaction body
```cpp

    #include "milecsa.hpp"
    #include "json.hpp"

    using json = nlohmann::json ;
    using transfer = milecsa::transaction::Transfer<json>;

    if (auto trx_body = transfer::CreateRequest(
                keyPair,
                destination,
                0, // block id
                0, // trx id
                0, // asset code
                "10")->get_body()){

         auto json_body = trx_body->dump();
    }
    else {
        //
        // Handle error
        //
    }
```

#### Create request of asset transfer in complete way
```cpp

    #include "milecsa.hpp"
    #include "json.hpp"

    using json = nlohmann::json ;

    milecsa::result result;

    //
    // Error handler
    //
    auto error = [&](milecsa::result code, const std::string &error) mutable -> void {
        //
        // Handle the error catch
        //
    };

    std::string transaction;
    std::string fee;
    std::string digest;
    std::string signature;

    if (auto transfer = milecsa::transaction::Transfer<json>::CreateRequest(
            keyPair,
            dstWalletPublicKey,
            blockId,
            transactionId,
            assetCode,
            amount,
            description,
            fee,
            error)) {

        //
        // getting json string
        //
        if (auto trx = transfer->get_body()) transaction = trx->dump();

        // transaction digest
        if (auto dgst = transfer->get_digest()) digest = *dgst;

        // transaction sigature
        if (auto sig = transfer->get_signature()) signature = *sig;

        //
        // Send json-rpc request to node...
        //
    }
```

#### XDR Emission request. Simple getting transaction body
```cpp

    #include "milecsa.hpp"
    #include "json.hpp"

    using json = nlohmann::json ;
    using emission = milecsa::transaction::Emission<json>;

    if (auto trx_body = emission::CreateRequest(
                keyPair,
                destination,
                0, // block id
                0, // trx id
                0, // asset code
                "100")->get_body()){

         auto json_body = trx_body->dump();
    }
    else {
        //
        // Handle error
        //
    }
```
