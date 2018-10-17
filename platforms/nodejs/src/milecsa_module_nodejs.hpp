//
// Created by lotus mile on 17/10/2018.
//

#ifndef PROJECT_MILECSA_PAIR_HPP
#define PROJECT_MILECSA_PAIR_HPP

#include <nan.h>
#include "milecsa.hpp"

namespace milecsa::nodejs {

    class Module : public Nan::ObjectWrap {
    public:

        static NAN_MODULE_INIT(Init);
        static NAN_METHOD(New);
        static NAN_GETTER(HandleGetters);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
    };
}

#endif //PROJECT_MILECSA_PAIR_HPP
