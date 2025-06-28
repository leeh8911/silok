#pragma once

#include <string>

#include "silok/domain/base_password_hasher.hpp"

namespace silok::infra
{
class CryptPasswordHasher : public ::silok::domain::BasePasswordHasher
{
 public:
    std::string HashPassword(const std::string& password) const override;
    bool CheckPassword(const std::string& password, const std::string& hash) const override;
};

class NoOpPasswordHasher : public ::silok::domain::BasePasswordHasher
{
 public:
    std::string HashPassword(const std::string& password) const override;
    bool CheckPassword(const std::string& password, const std::string& hash) const override;
};
}  // namespace silok::infra
