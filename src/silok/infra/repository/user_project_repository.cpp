#pragma once

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
int64_t UserProjectRepository::Create(const silok::domain::UserProject& relation)
{
    return infra::StorageManager::Insert(relation);
}

std::optional<silok::domain::UserProject> UserProjectRepository::FindById(int64_t id)
{
    auto relations = infra::StorageManager::FindByField<silok::domain::UserProject>(
        &silok::domain::UserProject::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<silok::domain::UserProject> UserProjectRepository::FindByFirstId(int64_t first_id)
{
    return infra::StorageManager::FindByField<silok::domain::UserProject>(
        &silok::domain::UserProject::first_id, first_id);
}

std::vector<silok::domain::UserProject> UserProjectRepository::FindBySecondId(int64_t second_id)
{
    return infra::StorageManager::FindByField<silok::domain::UserProject>(
        &silok::domain::UserProject::second_id, second_id);
}

bool UserProjectRepository::Update(const silok::domain::UserProject& relation)
{
    return infra::StorageManager::Update(relation);
}

void UserProjectRepository::Delete(int64_t id)
{
    silok::domain::UserProject relation =
        infra::StorageManager::FindByField<silok::domain::UserProject>(
            &silok::domain::UserProject::id, id)
            .front();
    infra::StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
