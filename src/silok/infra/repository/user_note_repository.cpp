#pragma once

#include "silok/infra/repository/user_note_repository.hpp"

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
int64_t UserNoteRepository::Create(const silok::domain::UserNote& relation)
{
    return infra::StorageManager::Insert(relation);
}

std::optional<silok::domain::UserNote> UserNoteRepository::FindById(int64_t id)
{
    auto relations = infra::StorageManager::FindByField<silok::domain::UserNote>(
        &silok::domain::UserNote::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<silok::domain::UserNote> UserNoteRepository::FindByFirstId(int64_t first_id)
{
    return infra::StorageManager::FindByField<silok::domain::UserNote>(
        &silok::domain::UserNote::first_id, first_id);
}

std::vector<silok::domain::UserNote> UserNoteRepository::FindBySecondId(int64_t second_id)
{
    return infra::StorageManager::FindByField<silok::domain::UserNote>(
        &silok::domain::UserNote::second_id, second_id);
}

bool UserNoteRepository::Update(const silok::domain::UserNote& relation)
{
    return infra::StorageManager::Update(relation);
}

void UserNoteRepository::Delete(int64_t id)
{
    silok::domain::UserNote relation = infra::StorageManager::FindByField<silok::domain::UserNote>(
                                           &silok::domain::UserNote::id, id)
                                           .front();
    infra::StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
