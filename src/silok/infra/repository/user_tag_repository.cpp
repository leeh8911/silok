#pragma once

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

int64_t UserTagRepository::Create(const silok::domain::UserTag& relation)
{
    return infra::StorageManager::Insert(relation);
}

std::optional<silok::domain::UserTag> UserTagRepository::FindById(int64_t id)
{
    auto relations =
        infra::StorageManager::FindByField<silok::domain::UserTag>(&silok::domain::UserTag::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<silok::domain::UserTag> UserTagRepository::FindByFirstId(int64_t first_id)
{
    return infra::StorageManager::FindByField<silok::domain::UserTag>(
        &silok::domain::UserTag::first_id, first_id);
}

std::vector<silok::domain::UserTag> UserTagRepository::FindBySecondId(int64_t second_id)
{
    return infra::StorageManager::FindByField<silok::domain::UserTag>(
        &silok::domain::UserTag::second_id, second_id);
}

bool UserTagRepository::Update(const silok::domain::UserTag& relation)
{
    return infra::StorageManager::Update(relation);
}

void UserTagRepository::Delete(int64_t id)
{
    silok::domain::UserTag relation =
        infra::StorageManager::FindByField<silok::domain::UserTag>(&silok::domain::UserTag::id, id)
            .front();
    infra::StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
