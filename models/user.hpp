#pragma once

#include <string>

#pragma db object
class User
{
 public:
#pragma db id auto
    unsigned long id;
    std::string username;
    User() = default;
    User(const std::string& name) : username(name) {}
};
