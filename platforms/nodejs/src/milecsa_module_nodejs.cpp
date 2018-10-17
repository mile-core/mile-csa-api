//
// Created by lotus mile on 17/10/2018.
//

#include <functional>
#include <optional>
#include "milecsa_module_nodejs.hpp"

namespace milecsa::nodejs {

    Nan::Persistent<v8::FunctionTemplate> Module::constructor;

    milecsa::ErrorHandler error_handler = [](milecsa::result code, std::string error){
        Nan::ThrowError(Nan::New("milecsa error: " + error).ToLocalChecked());
    };

    NAN_MODULE_INIT(Module::Init) {
            v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Module::New);
            constructor.Reset(ctor);
            ctor->InstanceTemplate()->SetInternalFieldCount(1);
            ctor->SetClassName(Nan::New("Module").ToLocalChecked());

            Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("Pair").ToLocalChecked(), Module::HandleGetters);

            target->Set(Nan::New("Module").ToLocalChecked(), ctor->GetFunction());
    }

    NAN_METHOD(Module::New) {
            Module* module = new Module();
            module->Wrap(info.Holder());
            info.GetReturnValue().Set(info.Holder());
    }

    static inline void __update_key(const std::optional<milecsa::keys::Pair> &pair, const Nan::FunctionCallbackInfo<v8::Value>& info){
        v8::Local<v8::Object> object = Nan::New<v8::Object>();

        v8::Local<v8::String> pk_name = Nan::New("public_key").ToLocalChecked();
        v8::Local<v8::Value> pk_value = Nan::New(pair->get_public_key().encode()).ToLocalChecked();

        v8::Local<v8::String> pvk_name = Nan::New("private_key").ToLocalChecked();
        v8::Local<v8::Value> pvk_value = Nan::New(pair->get_private_key().encode()).ToLocalChecked();

        Nan::Set(object, pk_name, pk_value);
        Nan::Set(object, pvk_name, pvk_value);

        info.GetReturnValue().Set(object);
    }

    void Random(const Nan::FunctionCallbackInfo<v8::Value>& info) {

        if (auto pair = milecsa::keys::Pair::Random(error_handler)){
            __update_key(pair,info);
        }
    }

    void WithSecret(const Nan::FunctionCallbackInfo<v8::Value>& info) {

        if(info.Length() != 1) {
            return Nan::ThrowError(Nan::New("milecsa error: WithSecret - expected secret phrase").ToLocalChecked());
        }

        if(!info[0]->IsString()) {
            return Nan::ThrowError(Nan::New("Vmilecsa error: WithSecret - expected argument string").ToLocalChecked());
        }

        v8::Local<v8::String> str = v8::Local<v8::String>::Cast(info[0]);
        v8::String::Utf8Value phrase(str);

        if (auto pair = milecsa::keys::Pair::WithSecret(*phrase, error_handler)){
            __update_key(pair,info);
        }
    }

    static inline bool __check_one_args(const Nan::FunctionCallbackInfo<v8::Value>& info, const std::string &name, const std::string &what){
        std::string prefix = "milecsa error: " + name + " - expected ";
        if(info.Length() != 1) {
            prefix += what;
            Nan::ThrowError(Nan::New(prefix).ToLocalChecked());
            return false;
        }

        if(!info[0]->IsString()) {
            prefix += "argument string";
            Nan::ThrowError(Nan::New(prefix).ToLocalChecked());
            return false;
        }

        return true;
    }

    void FromPrivateKey(const Nan::FunctionCallbackInfo<v8::Value>& info) {

        if(!__check_one_args(info,"FromPrivateKey","private key"))
            return;

        v8::Local<v8::String> str = v8::Local<v8::String>::Cast(info[0]);
        v8::String::Utf8Value pvk(str);

        if (auto pair = milecsa::keys::Pair::FromPrivateKey(*pvk, error_handler)){
            __update_key(pair,info);
        }
    }

    void ValidatePublicKey(const Nan::FunctionCallbackInfo<v8::Value>& info) {

        if(!__check_one_args(info,"ValidatePublicKey","public key"))
            return;

        v8::Local<v8::String> str = v8::Local<v8::String>::Cast(info[0]);
        v8::String::Utf8Value pk(str);

        if (milecsa::keys::Pair::ValidatePublicKey(*pk, error_handler)){
            info.GetReturnValue().Set(Nan::True());
        } else{
            info.GetReturnValue().Set(Nan::False());
        }
    }

    void ValidatePrivateKey(const Nan::FunctionCallbackInfo<v8::Value>& info) {
        
        if(!__check_one_args(info,"ValidatePrivateKey","private key"))
            return;

        v8::Local<v8::String> str = v8::Local<v8::String>::Cast(info[0]);
        v8::String::Utf8Value pvk(str);

        if (milecsa::keys::Pair::ValidatePrivateKey(*pvk, error_handler)){
            info.GetReturnValue().Set(Nan::True());
        } else{
            info.GetReturnValue().Set(Nan::False());
        }
    }

    static inline void __set_function(v8::Local<v8::Object> &object, const std::string name, const v8::Local<v8::FunctionTemplate> &tpl){
        v8::Local<v8::String> key = Nan::New(name).ToLocalChecked();
        v8::Local<v8::Function> fn = tpl->GetFunction();
        fn->SetName(Nan::New(name.c_str()).ToLocalChecked());
        Nan::Set(object, key, fn);
    }

    NAN_GETTER(Module::HandleGetters) {
            Module* self = Nan::ObjectWrap::Unwrap<Module>(info.This());

            std::string propertyName = std::string(*Nan::Utf8String(property));
            if (propertyName == "Pair") {

                v8::Local<v8::Object> object = Nan::New<v8::Object>();

                __set_function(object, "Random", Nan::New<v8::FunctionTemplate>(Random));
                __set_function(object, "WithSecret", Nan::New<v8::FunctionTemplate>(WithSecret));
                __set_function(object, "FromPrivateKey", Nan::New<v8::FunctionTemplate>(FromPrivateKey));
                __set_function(object, "ValidatePublicKey", Nan::New<v8::FunctionTemplate>(ValidatePublicKey));
                __set_function(object, "ValidatePrivateKey", Nan::New<v8::FunctionTemplate>(ValidatePrivateKey));

                info.GetReturnValue().Set(object);

            }  else {
                info.GetReturnValue().Set(Nan::Undefined());
            }
    }
}