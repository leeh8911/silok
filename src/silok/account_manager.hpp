#pragma once

#include <optional>
#include <string>

namespace silok
{

class AccountManager
{
 public:
    AccountManager() = default;

    void CreateAccount(const std::string& username, const std::string& password,
                       const std::string& email);

    std::optional<std::string> Login(const std::string& email, const std::string& password);
};
}  // namespace silok
