#pragma once

#include <string>

namespace silok::crypt
{
class BasePasswordHasher
{
 public:
    virtual ~BasePasswordHasher() = default;

    virtual std::string HashPassword(const std::string& password) const = 0;
    virtual bool CheckPassword(const std::string& password, const std::string& hash) const = 0;
};

class CryptPasswordHasher : public BasePasswordHasher
{
 public:
    std::string HashPassword(const std::string& password) const override;
    bool CheckPassword(const std::string& password, const std::string& hash) const override;
};

class NoOpPasswordHasher : public BasePasswordHasher
{
 public:
    std::string HashPassword(const std::string& password) const override;
    bool CheckPassword(const std::string& password, const std::string& hash) const override;
};
}  // namespace silok::crypt
