#pragma once
#include "../MileTest.h"
#include <string>
#include <iomanip>
#include <sstream>

#ifdef WITH_CURL
#include <curl/curl.h>
#else
#warning "curl library has not been configured"
#endif

using namespace std;

static auto error_handler = [](milecsa::result code, const std::string &error) mutable -> void {
    BOOST_TEST_MESSAGE("Error happened in MIleTestTransaction: " + error);
};


size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

struct MIleTestTransaction: public MileTest  {

    std::string url =  "http://lotus000.testnet.mile.global/v1/api";

    MIleTestTransaction(const std::string &name) : MileTest(name) {
        keyPair = milecsa::keys::Pair::WithSecret("secret-phrase", error_handler);
        if (!keyPair)
            return ;
        else
        {
            print_key_pair(keyPair);
        }

        destination = milecsa::keys::Pair::WithSecret("destination-secret-phrase", error_handler);
        if (!destination)
            return ;
        else
        {
            print_key_pair(destination);
        }
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

    int send(const string &body)
    {
        int ret = 0;
#ifdef WITH_CURL
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
#else
#warning "curl library has not been configured"
#endif
        return ret;
    }


    std::optional<milecsa::keys::Pair> keyPair;
    std::optional<milecsa::keys::Pair> destination;
    const std::string nodeAddress = "2n9z7C3f9SdCrLuCkekxFRgaFq8eoJMizRzbJDpxGoXnYgTaoz";

};

