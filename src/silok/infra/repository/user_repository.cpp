#include "silok/infra/repository/user_repository.hpp"

#include <optional>

#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

namespace silok::infra::repository
{

UserRepository::UserRepository()
{
    SILOK_LOG_INFO("UserRepository initialized");
}
int64_t UserRepository::Create(const silok::domain::User& user)
{
    return infra::StorageManager::Insert(user);
}

std::optional<silok::domain::User> UserRepository::FindByEmail(const std::string& email)
{
    auto users =
        infra::StorageManager::FindByField<silok::domain::User>(&silok::domain::User::email, email);
    if (users.empty())
    {
        return std::nullopt;
    }
    return users.front();
}

std::optional<silok::domain::User> UserRepository::FindById(int64_t id)
{
    auto users =
        infra::StorageManager::FindByField<silok::domain::User>(&silok::domain::User::id, id);
    if (users.empty())
    {
        return std::nullopt;
    }
    return users.front();
}

bool UserRepository::Update(const silok::domain::User& user)
{
    try
    {
        infra::StorageManager::Update(user);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
void UserRepository::Delete(int64_t id)
{
    silok::domain::User user =
        infra::StorageManager::FindByField<silok::domain::User>(&silok::domain::User::id, id)
            .front();
    infra::StorageManager::Remove(user);
}
}  // namespace silok::infra::repository
