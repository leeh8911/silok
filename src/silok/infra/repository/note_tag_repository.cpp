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

using silok::domain::NoteTag;
using silok::infra::StorageManager;

int64_t NoteTagRepository::Create(const NoteTag& relation)
{
    return StorageManager::Insert(relation);
}

std::optional<NoteTag> NoteTagRepository::FindById(int64_t id)
{
    auto relations = StorageManager::FindByField<NoteTag>(&NoteTag::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<NoteTag> NoteTagRepository::FindByFirstId(int64_t first_id)
{
    return StorageManager::FindByField<NoteTag>(&NoteTag::first_id, first_id);
}

std::vector<NoteTag> NoteTagRepository::FindBySecondId(int64_t second_id)
{
    return StorageManager::FindByField<NoteTag>(&NoteTag::second_id, second_id);
}

bool NoteTagRepository::Update(const NoteTag& relation)
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

void NoteTagRepository::Delete(int64_t id)
{
    NoteTag relation = StorageManager::FindByField<NoteTag>(&NoteTag::id, id).front();
    StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
