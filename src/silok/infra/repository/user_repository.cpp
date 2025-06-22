#include "silok/infra/repository/user_repository.hpp"

#include <optional>

#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

namespace silok::infra::repository
{

using silok::domain::User;
using silok::infra::StorageManager;

int64_t UserRepository::Create(const User& user)
{
    return StorageManager::Insert(user);
}

std::optional<User> UserRepository::FindByEmail(const std::string& email)
{
    auto users = StorageManager::FindByField<User>(&User::email, email);
    if (users.empty())
    {
        return std::nullopt;
    }
    return users.front();
}

std::optional<User> UserRepository::FindById(int64_t id)
{
    auto users = StorageManager::FindByField<User>(&User::id, id);
    if (users.empty())
    {
        return std::nullopt;
    }
    return users.front();
}

bool UserRepository::Update(const User& user)
{
    try
    {
        StorageManager::Update(user);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
void UserRepository::Delete(int64_t id)
{
    User user = StorageManager::FindByField<User>(&User::id, id).front();
    StorageManager::Remove(user);
}
}  // namespace silok::infra::repository
