#include "silok/infra/repository/user_tag_repository.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/repository/base_relationship_repository.hpp"
#include "silok/infra/storage_manager.hpp"

namespace silok::infra::repository
{

using silok::domain::UserTag;
using silok::infra::StorageManager;

int64_t UserTagRepository::Create(const UserTag& relation)
{
    return StorageManager::Insert(relation);
}

std::optional<UserTag> UserTagRepository::FindById(int64_t id)
{
    auto relations = StorageManager::FindByField<UserTag>(&UserTag::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<UserTag> UserTagRepository::FindByFirstId(int64_t first_id)
{
    return StorageManager::FindByField<UserTag>(&UserTag::first_id, first_id);
}

std::vector<UserTag> UserTagRepository::FindBySecondId(int64_t second_id)
{
    return StorageManager::FindByField<UserTag>(&UserTag::second_id, second_id);
}

bool UserTagRepository::Update(const UserTag& relation)
{
    try
    {
        StorageManager::Update(relation);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void UserTagRepository::Delete(int64_t id)
{
    UserTag relation = StorageManager::FindByField<UserTag>(&UserTag::id, id).front();
    StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
