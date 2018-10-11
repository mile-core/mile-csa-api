//
// Created by lotus mile on 03/10/2018.
//

#ifndef MILECSA_MILECSA_EMISSION_HPP
#define MILECSA_MILECSA_EMISSION_HPP

#include "milecsa_transfer.hpp"

namespace milecsa{
    namespace transaction{

        static const char  __emissionTransferName[] = "EmissionTransaction";

        /**
         * Emission request builder.
         * @tparam T
         */
        template <typename T>
        class Emission: public BaseTransfer<T,__emissionTransferName> {};
    }
}

#endif //MILECSA_MILECSA_EMISSION_HPP
