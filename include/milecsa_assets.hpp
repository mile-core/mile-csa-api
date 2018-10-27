//
// Created by denn on 27/10/2018.
//
#pragma once
#include <string>
#include "mile_crypto.h"

namespace milecsa {

    /**
    * Token description
    */
    struct token {
        unsigned short code;
        unsigned short precision;
        std::string name;

        template<typename T>
        /**
         * Prepare asset amount from asset code
         *
         * @tparam T
         * @param amount - amount
         * @param assetCode - asset code
         * @return prepared amount fixed point number presented as string
         */
        std::string value_to_string(T amount) const {
            //
            // truncate rest of the number
            //
            std::string value;
            float2FixedPoint((float)amount, precision, value);
            return value;
        }

        /**
         * Build tocken description
         *
         * @param code
         * @param precision
         * @param name
         */
        token(unsigned short code, unsigned short precision, const std::string &name);

        token(const token &);

    private:
        token();
    };

    /**
    * Available blockchain tokens
    */
    struct assets {
        /**
         * Stable coin
         */
        static token XDR;

        /**
         * Index coin
         */
        static token MILE;

        /**
        * NULL coin
        */
        static token NILL;
    };
}