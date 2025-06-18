#include "silok/application/user_service.hpp"

#include <optional>
#include <string>

#include "silok/domain/user.hpp"

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
    // Implementation for creating a user
}

std::optional<domain::User> UserService::FindByEmail(const std::string& email)
{
    // Implementation for finding a user by email
    return std::nullopt;  // Placeholder
}

}  // namespace silok::application
