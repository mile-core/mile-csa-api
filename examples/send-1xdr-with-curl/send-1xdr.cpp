//
// Created by lotus mile on 20/10/2018.
//
// Example shows how to prepare MILE transaction and then send to node use MILE JSON-RPC API
// Alternate C++ wrappper for json-rpc API: https://github.com/mile-core/mile-csa-jsonrpc-client
//
//

#include <optional>
#include <string>
#include <iostream>
#include "milecsa.hpp"

#ifdef WITH_CURL
#include <curl/curl.h>
#else
#error "curl library has not been configured"
#endif

using milecsa::keys::Pair;
using result   = milecsa::result;
using transfer = milecsa::transaction::JsonTransfer;

using namespace milecsa::transaction;
using namespace milecsa::keys;

using namespace std;

string url = "http://104.248.7.135:8080/v1/api";

///
/// Get last block id from node with JSON-RPC request uses curl lib
///
uint256_t get_block_id();

///
/// Send prepared transaction body
///
int send_transaction(const string &body);

///
/// Common error closure
///
auto error_handler = [](result code, const string &error){
    cerr << "MILE error: " << code << " : " << error << endl;
    exit(-1);
};

int main(int argc, char *argv[]) {

    if (argc != 3) {
        cerr << "Usage to send 1 XDR from -> to" << endl;
        cerr << argv[0] << " <from address as secret phrase> <to>" << endl;
        return -1;
    }

    string to   = argv[2];
    string from = argv[1];

    ///
    /// Validate desination address
    ///
    if(!Pair::ValidatePublicKey(to, error_handler))
        return -1;

    uint256_t block_id = get_block_id();

    ///
    /// trx id can be create for user reason or can be default
    ///
    uint64_t trx_id = rand();

    ///
    /// Create random pair
    ///
    optional<Pair>   pair = Pair::WithSecret(from, error_handler);

    ///
    /// Some fails occurred
    ///
    if (!pair)
        return -1;

    cout << " From: " << endl;
    cout << "         private key = " << pair->get_private_key().encode() << endl;
    cout << "          public key = " << pair->get_public_key().encode() << endl;

    cout << " To: " << endl;
    cout << "          public key = " << to << endl;

    if(auto request = transfer::CreateRequest(
            *pair,
            to,
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

        ///
        /// Send transfer
        ///
        return  send_transaction(request->get_body()->dump());
    }

}


//
// Curl utils
//
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

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

uint256_t get_block_id(){

    uint256_t block_id = (uint256_t)-1;
#ifdef WITH_CURL

    string request = "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"get-current-block-id\",\"params\":null,\"version\":0.0}";

    CURL *curl;
    CURLcode res;
    ///
    /// Create curl object
    ///
    curl = curl_easy_init();

    if(curl) {

        std::cout << std::endl << "Response:" << std::endl;

        ///
        /// Setup curl request
        ///
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)request.length());

        std::string response_string;
        std::string header_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

        ///
        /// Perform the request, res will get the return code
        ///

        res = curl_easy_perform(curl);

        ///
        /// Check for errors
        ///
        if(res != CURLE_OK){
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;

            cerr << " ------ " << endl;
            cerr << " : " << header_string << endl;
            cerr << " : " << response_string << endl;
            cerr << " -------" << endl;

        }
        else {
            cerr << " : " << response_string << endl;

            nlohmann::json json = nlohmann::json::parse(response_string);

            nlohmann::json result = json.at("result");

            if (!result.empty()) {
                std::string r = result["current-block-id"];
                if(!StringToUInt256(r, block_id, false)){
                    error_handler(milecsa::result::FAIL, " could covert " + r + " to uint256");
                    return false;
                }
            }
        }

        ///
        /// Always cleanup
        ///
        curl_easy_cleanup(curl);
    }
#else
#warning "curl library has not been configured"
#endif
    return block_id;
}
