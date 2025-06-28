#pragma once

#include <memory>
#include <optional>
#include <string>

#include "silok/crypt/password_hasher.hpp"
#include "silok/model.hpp"

namespace silok::manager
{

class AccountManager
{
 public:
    explicit AccountManager(std::shared_ptr<crypt::BasePasswordHasher> password_hasher);

    void CreateAccount(const std::string& username, const std::string& password,
                       const std::string& email);

    std::optional<std::string> Login(const std::string& email, const std::string& password) const;

    std::optional<User> GetAccountInfo(const std::string& token) const;

    void DeleteAccount(const User& user, const std::string& token);

 private:
    std::shared_ptr<crypt::BasePasswordHasher> password_hasher{nullptr};
};
}  // namespace silok::manager
