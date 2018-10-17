//
// Created by lotus mile on 17/10/2018.
//

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

    void Random(const Nan::FunctionCallbackInfo<v8::Value>& info) {

        if (auto pair = milecsa::keys::Pair::Random(error_handler)){

            v8::Local<v8::Object> object = Nan::New<v8::Object>();

            v8::Local<v8::String> pk_name = Nan::New("public_key").ToLocalChecked();
            v8::Local<v8::Value> pk_value = Nan::New(pair->get_public_key().encode()).ToLocalChecked();

            v8::Local<v8::String> pvk_name = Nan::New("private_key").ToLocalChecked();
            v8::Local<v8::Value> pvk_value = Nan::New(pair->get_private_key().encode()).ToLocalChecked();

            Nan::Set(object, pk_name, pk_value);
            Nan::Set(object, pvk_name, pvk_value);

            info.GetReturnValue().Set(object);
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

            v8::Local<v8::Object> object = Nan::New<v8::Object>();

            v8::Local<v8::String> pk_name = Nan::New("public_key").ToLocalChecked();
            v8::Local<v8::Value> pk_value = Nan::New(pair->get_public_key().encode()).ToLocalChecked();

            v8::Local<v8::String> pvk_name = Nan::New("private_key").ToLocalChecked();
            v8::Local<v8::Value> pvk_value = Nan::New(pair->get_private_key().encode()).ToLocalChecked();

            Nan::Set(object, pk_name, pk_value);
            Nan::Set(object, pvk_name, pvk_value);

            info.GetReturnValue().Set(object);
        }
    }

    void FromPrivateKey(const Nan::FunctionCallbackInfo<v8::Value>& info) {

        if(info.Length() != 1) {
            return Nan::ThrowError(Nan::New("milecsa error: FromPrivateKey - expected private key").ToLocalChecked());
        }

        if(!info[0]->IsString()) {
            return Nan::ThrowError(Nan::New("Vmilecsa error: FromPrivateKey - expected argument string").ToLocalChecked());
        }

        v8::Local<v8::String> str = v8::Local<v8::String>::Cast(info[0]);
        v8::String::Utf8Value pvk(str);

        if (auto pair = milecsa::keys::Pair::FromPrivateKey(*pvk, error_handler)){

            v8::Local<v8::Object> object = Nan::New<v8::Object>();

            v8::Local<v8::String> pk_name = Nan::New("public_key").ToLocalChecked();
            v8::Local<v8::Value> pk_value = Nan::New(pair->get_public_key().encode()).ToLocalChecked();

            v8::Local<v8::String> pvk_name = Nan::New("private_key").ToLocalChecked();
            v8::Local<v8::Value> pvk_value = Nan::New(pair->get_private_key().encode()).ToLocalChecked();

            Nan::Set(object, pk_name, pk_value);
            Nan::Set(object, pvk_name, pvk_value);

            info.GetReturnValue().Set(object);
        }
    }


    NAN_GETTER(Module::HandleGetters) {
            Module* self = Nan::ObjectWrap::Unwrap<Module>(info.This());

            std::string propertyName = std::string(*Nan::Utf8String(property));
            if (propertyName == "Pair") {

                v8::Local<v8::Object> object = Nan::New<v8::Object>();

                v8::Local<v8::String> rnd = Nan::New("Random").ToLocalChecked();
                v8::Local<v8::FunctionTemplate> tpl_rnd = Nan::New<v8::FunctionTemplate>(Random);
                v8::Local<v8::Function> fn_rnd = tpl_rnd->GetFunction();
                fn_rnd->SetName(Nan::New("Random").ToLocalChecked());

                v8::Local<v8::String> secret = Nan::New("WithSecret").ToLocalChecked();
                v8::Local<v8::FunctionTemplate> tpl_secret = Nan::New<v8::FunctionTemplate>(WithSecret);
                v8::Local<v8::Function> fn_secret = tpl_secret->GetFunction();
                fn_secret->SetName(Nan::New("WithSecret").ToLocalChecked());

                v8::Local<v8::String> private_k = Nan::New("FromPrivateKey").ToLocalChecked();
                v8::Local<v8::FunctionTemplate> tpl_private_k = Nan::New<v8::FunctionTemplate>(FromPrivateKey);
                v8::Local<v8::Function> fn_private_k = tpl_private_k->GetFunction();
                fn_private_k->SetName(Nan::New("FromPrivateKey").ToLocalChecked());

                Nan::Set(object, rnd, fn_rnd);
                Nan::Set(object, secret, fn_secret);
                Nan::Set(object, private_k, fn_private_k);

                info.GetReturnValue().Set(object);

            }  else {
                info.GetReturnValue().Set(Nan::Undefined());
            }
    }
}