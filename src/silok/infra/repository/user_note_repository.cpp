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
using silok::domain::UserNote;
using silok::infra::StorageManager;

int64_t UserNoteRepository::Create(const UserNote& relation)
{
    return StorageManager::Insert(relation);
}

std::optional<UserNote> UserNoteRepository::FindById(int64_t id)
{
    auto relations = StorageManager::FindByField<UserNote>(&UserNote::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<UserNote> UserNoteRepository::FindByFirstId(int64_t first_id)
{
    return StorageManager::FindByField<UserNote>(&UserNote::first_id, first_id);
}

std::vector<UserNote> UserNoteRepository::FindBySecondId(int64_t second_id)
{
    return StorageManager::FindByField<UserNote>(&UserNote::second_id, second_id);
}

bool UserNoteRepository::Update(const UserNote& relation)
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

void UserNoteRepository::Delete(int64_t id)
{
    UserNote relation = StorageManager::FindByField<UserNote>(&UserNote::id, id).front();
    StorageManager::Remove(relation);
}
}  // namespace silok::infra::repository
