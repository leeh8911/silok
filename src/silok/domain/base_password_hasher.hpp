#pragma once

#include <memory>
#include <string>

namespace silok::domain
{
class BasePasswordHasher
{
 public:
    virtual ~BasePasswordHasher() = default;

    virtual std::string HashPassword(const std::string& password) const = 0;
    virtual bool CheckPassword(const std::string& password, const std::string& hash) const = 0;
};

using PasswordHasherPtr = std::shared_ptr<BasePasswordHasher>;

}  // namespace silok::domain
