//
// Created by lotus mile on 20/10/2018.
//
// Example shows how to prepare MILE transaction adn then send to node use MILE JSON-RPC API
// Alternate C++ wrappper for json-rpc API: https://github.com/mile-core/mile-csa-jsonrpc-client
//
//

#include <string>
#include <iostream>
#include "../../platforms/wasm/wrapper/milecsa_light_api.hpp"

#ifdef WITH_CURL
#include <curl/curl.h>
#else
#error "curl library has not been configured"
#endif

using milecsa::light::Pair;
using  result = milecsa::light::result;
using namespace milecsa::transaction;
using namespace milecsa::keys;
using namespace std;

string url = "http://104.248.7.135/v1/api";

int send_transaction(const string &body)
{
    int ret = 0;

    CURL *curl;
    CURLcode res;

    string request = "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"send-transaction\",\"version\":0.0, \"params\": ";
    request += body + "}";

    cout << "Send: " << endl;
    cout << "      " << request << endl;

    ///
    /// Create curl object
    ///
    curl = curl_easy_init();

    if(curl) {

        cout << endl << "Response:" << endl;

        ///
        /// Setup curl request
        ///
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)request.length());

        ///
        /// Perform the request, res will get the return code
        ///

        res = curl_easy_perform(curl);

        ///
        /// Check for errors
        ///
        if(res != CURLE_OK){
            ret = -1;
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }

        ///
        /// Always cleanup
        ///
        curl_easy_cleanup(curl);
    }

    return ret;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        cerr << "Usage to send 1 XDR from -> to" << endl;
        cerr << argv[0] << " <from address as secret phrase> <to> <block id>" << endl;
        return -1;
    }

    string block_id = argv[3];
    string from = argv[1];
    string to = argv[2];
    string errorMessage;

    milecsa::light::Pair pair;

    ///
    /// Restore wallet private and public keys pair from secret phrase
    ///
    if (generate_with_secret(pair, from, errorMessage) != result::OK) {
        cerr << " from secret phrase error: " << errorMessage << endl;
        return  -1;
    }

    ///
    /// Validate destination address
    ///
    if (validate_public_key(to, errorMessage) != result::OK) {
        cerr << " destination public key error: " << errorMessage << endl;
        return  -1;
    }

    ///
    /// trx id can be create for user reason or can be default
    ///
    uint64_t trx_id = default_transaction_id;

    ///
    /// Transaction container string
    ///
    std::string transaction;

    ///
    /// Transaction container uniq digest
    ///
    std::string digest;

    ///
    /// Build signed transfer transaction
    ///

    milecsa::token token = milecsa::assets::XDR;
    std::string amount = milecsa::asset::amount_to_string(10.0f, milecsa::asset::code::XDR);


    if (milecsa::transaction::prepare_transfer(
            pair.private_key,          /// "from" private key
            to,                        /// "to" public key
            block_id,                  /// block id
            trx_id,                    /// user defined transaction id or number
            milecsa::asset::XDR,       /// asset code
            "1.00000",                 /// amount of transfer
            "memo",                    /// description
            "0.00000",                 /// empty fee

            transaction,               /// returned signed transaction as json string
            digest,                    /// uinq transaction digest string

            errorMessage               /// error message if something failed
    )){

        cerr << " prepare_transfer error: " << errorMessage << endl;
        return -1;
    }

    ///
    /// Send transfer
    ///
    return  send_transaction(transaction);
}