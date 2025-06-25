#pragma once

#include <optional>
#include <string>

#include "silok/model.hpp"

namespace silok::manager
{

class AccountManager
{
 public:
    AccountManager() = default;

    void CreateAccount(const std::string& username, const std::string& password,
                       const std::string& email);

    std::optional<std::string> Login(const std::string& email, const std::string& password) const;

    std::optional<User> GetAccountInfo(const std::string& token) const;

    void DeleteAccount(const User& user, const std::string& token);
};
}  // namespace silok::manager
