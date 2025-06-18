#pragma once

#include "silok/domain/user.hpp"

namespace silok::domain
{
class BaseUserRepository
{
 public:
    virtual ~BaseUserRepository() = default;

    virtual void create(const std::string& name, const std::string& email,
                        const std::string& password) = 0;
    virtual User findByEmail(const std::string& email) const = 0;
};

}  // namespace silok::domain
