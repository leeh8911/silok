#include "silok/application/user_service.hpp"

#include <optional>
#include <string>

#include "silok/domain/crypt.hpp"
#include "silok/domain/user.hpp"
#include "silok/domain/user_token.hpp"
#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

namespace silok::application
{
UserService& UserService::Get()
{
    static UserService instance;
    return instance;
}

void UserService::Create(const std::string& name, const std::string& email,
                         const std::string& password)
{
    domain::User user;
    user.name = name;
    user.email = email;
    user.password = domain::HashPassword(password);

    infra::StorageManager::Insert(user);
}

std::optional<domain::User> UserService::FindByEmail(const std::string& email)
{
    auto result = infra::StorageManager::FindByField(&domain::User::email, email);

    if (!result.empty())
    {
        return result.front();
    }

    return std::nullopt;
}

std::optional<std::string> UserService::Login(const std::string& email, const std::string& password)
{
    auto user = FindByEmail(email);
    if (user.has_value() && domain::CheckPassword(password, user->password))
    {
        SILOK_LOG_DEBUG("Login successful for user: {}", user->name);
        return domain::EncodeUserToken(user->id);
    }
    return std::nullopt;
}

}  // namespace silok::application
