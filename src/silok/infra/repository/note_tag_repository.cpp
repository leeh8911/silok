#pragma once

#include "silok/infra/repository/note_tag_repository.hpp"

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

int64_t NoteTagRepository::Create(const silok::domain::NoteTag& relation)
{
    return infra::StorageManager::Insert(relation);
}

std::optional<silok::domain::NoteTag> NoteTagRepository::FindById(int64_t id)
{
    auto relations =
        infra::StorageManager::FindByField<silok::domain::NoteTag>(&silok::domain::NoteTag::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<silok::domain::NoteTag> NoteTagRepository::FindByFirstId(int64_t first_id)
{
    return infra::StorageManager::FindByField<silok::domain::NoteTag>(
        &silok::domain::NoteTag::first_id, first_id);
}

std::vector<silok::domain::NoteTag> NoteTagRepository::FindBySecondId(int64_t second_id)
{
    return infra::StorageManager::FindByField<silok::domain::NoteTag>(
        &silok::domain::NoteTag::second_id, second_id);
}

bool NoteTagRepository::Update(const silok::domain::NoteTag& relation)
{
    return infra::StorageManager::Update(relation);
}

void NoteTagRepository::Delete(int64_t id)
{
    silok::domain::NoteTag relation =
        infra::StorageManager::FindByField<silok::domain::NoteTag>(&silok::domain::NoteTag::id, id)
            .front();
    infra::StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
