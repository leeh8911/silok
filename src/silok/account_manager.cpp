#include "silok/account_manager.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "silok/storage_manager.hpp"

namespace silok
{
void AccountManager::CreateAccount(const std::string& username, const std::string& password,
                                   const std::string& email)
{
    User user;
    user.name = username;
    user.email = email;
    user.password = password;

    // Insert the new user into the storage
    StorageManager::Insert(user);
}

std::optional<std::string> AccountManager::Login(const std::string& email,
                                                 const std::string& password)
{
    try
    {
        auto user = StorageManager::FindByField<User>(&User::email, email).front();

        if (user.password == password)
        {
            // In a real application, you would generate a token here
            return "dummy_token";  // Placeholder for a real token
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
}  // namespace silok
