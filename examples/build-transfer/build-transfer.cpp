//
// Created by denn on 19/10/2018.
//

///
/// This example shows way to prepare paremeters and build transfer transaction.
/// Transaction builder prepare body request contains digest of transactions and their
/// signature.
/// Then ths body can be send by any way to node for the next few blocks.
///

#include <optional>
#include <string>
#include <iostream>
#include <milecsa.hpp>

using milecsa::keys::Pair;
using result   = milecsa::result;
using transfer = milecsa::transaction::JsonTransfer;

using namespace milecsa::transaction;
using namespace milecsa::keys;

using namespace std;

int main(int argc, char *argv[]) {

    ///
    /// Common error closure
    ///
    auto error_handler = [](result code, const string &error){
        cerr << "MILE error: " << code << " : " << error << endl;
        exit(-1);
    };

    ///
    /// block id should be getting from blockchain node, for example as json-rpc request by curl
    /// curl  -H "Content-Type: application/json"  -d '{"id":0,"jsonrpc":"2.0","method":"get-current-block-id","version":0.0}' http://node002.testnet.mile.global/v1/api
    ///
    /// or any another way
    ///
    ///
    uint256_t block_id = 34923;

    ///
    /// trx id can be create for user reason or can be default
    ///
    uint64_t trx_id = rand();

    ///
    /// Create random pair
    ///
    optional<Pair>    pair = Pair::Random(error_handler);

    ///
    /// Some fails occurred
    ///
    if (!pair)
        return -1;

    cout << " Wallet: " << endl;
    cout << "         private key = " << pair->get_private_key().encode() << endl;
    cout << "          public key = " << pair->get_public_key().encode() << endl;

    if (auto destination = Pair::WithSecret("some secret phase!", error_handler)) {

        cout << " Wallet with secret: " << endl;
        cout << "         private key = " << destination->get_private_key().encode() << endl;
        cout << "          public key = " << destination->get_public_key().encode() << endl;

        if(auto request = transfer::CreateRequest(
                *pair,
                destination->get_public_key().encode(),
                block_id,
                trx_id,
                milecsa::assets::XDR,
                1,
                0,
                "memo",
                error_handler
                )){
            cout << " Signed transactions json digest : " << request->get_digest().value_or("wrong digest") << endl;
            cout << " Signed transactions json body   : " << endl;
            cout << request->get_body()->dump() << endl;
            return  0;
        }
    }

    return -1;
}