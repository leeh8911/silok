#pragma once

#include <memory>
#include <optional>
#include <string>

#include "silok/domain/base_password_hasher.hpp"
#include "silok/domain/model.hpp"

namespace silok::infra
{

class AccountManager
{
 public:
    explicit AccountManager(silok::domain::PasswordHasherPtr password_hasher);

    void CreateAccount(const std::string& username, const std::string& password,
                       const std::string& email);

    std::optional<std::string> Login(const std::string& email, const std::string& password) const;

    std::optional<User> GetAccountInfo(const std::string& token) const;

    void DeleteAccount(const User& user, const std::string& token);

 private:
    silok::domain::PasswordHasherPtr password_hasher{nullptr};
};
}  // namespace silok::infra
