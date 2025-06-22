#include "silok/infra/repository/user_project_repository.hpp"

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

using silok::domain::UserProject;
using silok::infra::StorageManager;

int64_t UserProjectRepository::Create(const UserProject& relation)
{
    return StorageManager::Insert(relation);
}

std::optional<UserProject> UserProjectRepository::FindById(int64_t id)
{
    auto relations = StorageManager::FindByField<UserProject>(&UserProject::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<UserProject> UserProjectRepository::FindByFirstId(int64_t first_id)
{
    return StorageManager::FindByField<UserProject>(&UserProject::first_id, first_id);
}

std::vector<UserProject> UserProjectRepository::FindBySecondId(int64_t second_id)
{
    return StorageManager::FindByField<UserProject>(&UserProject::second_id, second_id);
}

bool UserProjectRepository::Update(const UserProject& relation)
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

void UserProjectRepository::Delete(int64_t id)
{
    UserProject relation = StorageManager::FindByField<UserProject>(&UserProject::id, id).front();
    StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
