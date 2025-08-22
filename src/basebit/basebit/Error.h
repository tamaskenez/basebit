#pragma once

#include "basebit/export.h"

#include <stdexcept>
#include <string>

namespace basebit
{
class BASEBIT_EXPORT Error : std::runtime_error
{
public:
    explicit Error(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
    explicit Error(const char* msg)
        : std::runtime_error(msg)
    {
    }
};
} // namespace basebit
