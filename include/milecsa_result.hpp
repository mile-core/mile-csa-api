#ifndef MILECSA_TYPES_HPP
#define MILECSA_TYPES_HPP

namespace milecsa {

    /**
     * Common mile csa handling codes
     *
     * */
    typedef enum {
        UNKNOWN = -1,
        OK = 0,
        FAIL = 1,
        EMPTY = 2,
        NOT_SUPPORTED = 3,
        NOT_FOUND = 4,
        ALREADY_EXIST = 5,
        EXCEPTION = 6,
        TIMEOUT = 7,
        LAST = TIMEOUT
    } result;
}

#endif // MILECSA_TYPES_H