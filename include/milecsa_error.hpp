//
// Created by lotus mile on 03/10/2018.
//

#pragma once
#include <string>
#include <optional>
#include <functional>
#include "milecsa_result.hpp"

namespace milecsa {
    /**
     * Common error handling closure.
     * */
    typedef std::function<void(result code, const std::string &errorMessage)> ErrorHandler;

    /**
     * Default error handler
     */
    static auto default_error_handler = [](milecsa::result code, const std::string &error) {};
}
