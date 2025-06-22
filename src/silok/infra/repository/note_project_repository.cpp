#include "silok/infra/repository/note_project_repository.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::infra::repository
{

using silok::domain::NoteProject;
using silok::infra::StorageManager;

int64_t NoteProjectRepository::Create(const NoteProject& relation)
{
    return StorageManager::Insert(relation);
}

std::optional<NoteProject> NoteProjectRepository::FindById(int64_t id)
{
    auto relations = StorageManager::FindByField<NoteProject>(&NoteProject::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<NoteProject> NoteProjectRepository::FindByFirstId(int64_t first_id)
{
    return StorageManager::FindByField<NoteProject>(&NoteProject::first_id, first_id);
}

std::vector<NoteProject> NoteProjectRepository::FindBySecondId(int64_t second_id)
{
    return StorageManager::FindByField<NoteProject>(&NoteProject::second_id, second_id);
}

bool NoteProjectRepository::Update(const NoteProject& relation)
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

void NoteProjectRepository::Delete(int64_t id)
{
    NoteProject relation = StorageManager::FindByField<NoteProject>(&NoteProject::id, id).front();
    StorageManager::Remove(relation);
}

}  // namespace silok::infra::repository
