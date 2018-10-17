#include <node.h>
#include "milecsa_module_nodejs.hpp"

namespace milecsa::nodejs  {

    NAN_MODULE_INIT(InitModule) {
            Module::Init(target);
    }

    NODE_MODULE(milecsa, InitModule);

}