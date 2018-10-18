//
// Created by lotus mile on 17/10/2018.
//

#ifndef PROJECT_MILECSA_PAIR_HPP
#define PROJECT_MILECSA_PAIR_HPP

#include <nan.h>
#include "milecsa.hpp"

namespace milecsa::nodejs {

    namespace detail {

        class Transaction : public Nan::ObjectWrap {
        public:

            static inline Nan::Persistent<v8::Function> & constructor() {
                static Nan::Persistent<v8::Function> my_constructor;
                return my_constructor;
            }

            static NAN_METHOD(New);
            static NAN_METHOD(Transfer);
            static NAN_METHOD(Emission);

            static void init_class() {
                v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
                tpl->SetClassName(Nan::New("Transaction").ToLocalChecked());
                tpl->InstanceTemplate()->SetInternalFieldCount(1);
                Nan::SetPrototypeMethod(tpl, "Transfer", Transfer);
                Nan::SetPrototypeMethod(tpl, "Emission", Emission);
                constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            }
        };
    }

    class Module : public Nan::ObjectWrap {
    public:

        static NAN_MODULE_INIT(Init);
        static NAN_METHOD(New);
        static NAN_METHOD(Transaction);
        static NAN_GETTER(HandleGetters);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
    };
}

#endif //PROJECT_MILECSA_PAIR_HPP
