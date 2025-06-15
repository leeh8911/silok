#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "silok/data.hpp"

namespace silok::data
{
Role stringToRole(const std::string& role_str)
{
    if (role_str == "owner")
        return Role::Owner;
    else if (role_str == "shared")
        return Role::Shared;
    else
        throw std::invalid_argument("Invalid role string: " + role_str);
}

std::string roleToString(Role role)
{
    switch (role)
    {
        case Role::Owner:
            return "owner";
        case Role::Shared:
            return "shared";
        default:
            throw std::invalid_argument("Invalid role enum value");
    }
}

}  // namespace silok::data
