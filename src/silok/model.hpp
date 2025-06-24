#pragma once

#include <cstdint>
#include <string>

namespace silok
{
struct User
{
    int64_t id;
    std::string name;
    std::string email;
    std::string password;
};

}  // namespace silok
