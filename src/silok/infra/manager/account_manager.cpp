#include "account_manager.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "silok/domain/base_password_hasher.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/jwt_utils.hpp"
#include "silok/model.hpp"

namespace silok::infra
{

using silok::domain::PasswordHasherPtr;

AccountManager::AccountManager(PasswordHasherPtr password_hasher_)
    : password_hasher(std::move(password_hasher_))
{
    if (!this->password_hasher)
    {
        throw std::runtime_error("Password hasher is not initialized");
    }
}

void AccountManager::CreateAccount(const std::string& username, const std::string& password,
                                   const std::string& email)
{
    auto existing_users = StorageManager::FindByField<User>(&User::email, email);
    if (!existing_users.empty())
    {
        throw std::runtime_error("Email already exists");
    }

    User user;
    user.name = username;
    user.email = email;
    user.password = this->password_hasher->HashPassword(password);

    // Insert the new user into the storage
    StorageManager::Insert(user);
}

std::optional<std::string> AccountManager::Login(const std::string& email,
                                                 const std::string& password) const
{
    try
    {
        auto user = StorageManager::FindByField<User>(&User::email, email).front();

        if (this->password_hasher->CheckPassword(password, user.password))
        {
            // In a real application, you would generate a token here
            return EncodeUserToken(user.id);
        }
        else
        {
            throw std::runtime_error("Invalid password");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return std::nullopt;  // User not found
    }
}

std::optional<User> AccountManager::GetAccountInfo(const std::string& token) const
{
    auto user_id = DecodeUserToken(token);

    if (user_id.has_value())
    {
        auto users = StorageManager::FindByField<User>(&User::id, user_id.value());
        if (!users.empty())
        {
            return users.front();
        }
        else
        {
            std::cerr << "User not found for ID: " << user_id.value() << '\n';
        }
    }
    else
    {
        std::cerr << "Invalid token\n";
    }
    return std::optional<User>();
}

void AccountManager::DeleteAccount(const User& user, const std::string& token)
{
    auto user_id = DecodeUserToken(token);
    if (user_id.has_value() && user_id.value() == user.id)
    {
        StorageManager::Remove(user);
    }
    else
    {
        std::cerr << "Invalid token\n";
    }
}
}  // namespace silok::infra
