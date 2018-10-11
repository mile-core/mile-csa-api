#include <stdarg.h>
#include <cstring>

#include "milecsa_utils.hpp"

const std::string StringFormat(const char* format, ...)
{
    char buffer[1024] = {};
    va_list ap = {};

    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    return std::string(buffer);
}

const std::string ErrorFormat(const char* format, ...)
{
    char buffer[1024] = {};
    va_list ap = {};

    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    return "MileCsa error: " + std::string(buffer);
}
