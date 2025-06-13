#pragma once

#include "silok/logger.hpp"

#include <stdexcept>
#include <string>

namespace silok
{
class Role
{
 public:
    enum class Type
    {
        Owner,
        Shared,
        Unknown
    };

    Role(Type role_ = Type::Unknown) : role(role_) {}

    static std::string toString(Type role)
    {
        switch (role)
        {
            case Type::Owner:
                return "owner";
            case Type::Shared:
                return "shared";
            default:
                return "unknown";
        }
    }

    std::string toString() const
    {
        return toString(role);
    }

    static Type fromString(const std::string& str)
    {
        if (str == "owner")
        {
            return Type::Owner;
        }
        else if (str == "shared")
        {
            return Type::Shared;
        }
        else if (str == "unknown")
        {
            return Type::Unknown;
        }
        else
        {
            throw std::invalid_argument("Invalid role string");
        }
    }

 private:
    Type role;
};

}  // namespace silok
