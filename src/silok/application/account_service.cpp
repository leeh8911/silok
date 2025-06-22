#include "silok/application/account_service.hpp"

#include <optional>
#include <string>

#include "silok/domain/crypt.hpp"
#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/repository/base_user_repository.hpp"
#include "silok/domain/user_token.hpp"
#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

namespace silok::application
{

AccountService::AccountService(silok::domain::repository::UserRepositoryPtr user_repository_)
    : user_repository(std::move(user_repository_))
{
    if (!user_repository)
    {
        SILOK_LOG_ERROR("UserRepository is not initialized");
        throw std::runtime_error("UserRepository is not initialized");
    }
}

void AccountService::Create(const std::string& name, const std::string& email,
                            const std::string& password)
{
    auto found = this->user_repository->FindByEmail(email);
    if (found.has_value())
    {
        SILOK_LOG_ERROR("User with email {} already exists", email);
        throw std::runtime_error("User with this email already exists");
    }

    domain::User user;
    user.name = name;
    user.email = email;
    user.password = domain::HashPassword(password);

    this->user_repository->Create(user);
}

std::optional<std::string> AccountService::Login(const std::string& email,
                                                 const std::string& password)
{
    auto user = this->user_repository->FindByEmail(email);
    if (user.has_value() && domain::CheckPassword(password, user->password))
    {
        SILOK_LOG_DEBUG("Login successful for user: {}", user->name);
        return domain::EncodeUserToken(user->id);
    }
    return std::nullopt;
}

void AccountService::Update(const domain::User& user)
{
    if (user.id <= 0)
    {
        SILOK_LOG_ERROR("Invalid user ID for update: {}", user.id);
        return;
    }
    auto upated_user = user;

    auto existing_user = this->user_repository->FindById(user.id);
    if (!existing_user.has_value())
    {
        SILOK_LOG_ERROR("User not found for update: {}", user.id);
        return;
    }

    if (existing_user.value().password != user.password)
    {
        SILOK_LOG_DEBUG("Password change detected for user: {}", user.name);
        upated_user.password = domain::HashPassword(user.password);
    }

    this->user_repository->Update(upated_user);
    SILOK_LOG_DEBUG("User updated: {}", upated_user.name);
}

std::optional<domain::User> AccountService::FindByEmail(const std::string& email)
{
    auto user = this->user_repository->FindByEmail(email);
    if (!user.has_value())
    {
        SILOK_LOG_ERROR("User not found with email: {}", email);
        return std::nullopt;
    }
    SILOK_LOG_DEBUG("User found: {}", user->name);
    return user;
}

void AccountService::Delete(const domain::User& user)
{
    auto existing_user = this->user_repository->FindById(user.id);
    if (!existing_user.has_value())
    {
        SILOK_LOG_ERROR("User not found for deletion: {}", user.id);
        return;
    }

    this->user_repository->Delete(user.id);
    SILOK_LOG_DEBUG("User deleted: {}", user.name);
}
}  // namespace silok::application
