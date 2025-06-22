#include "silok/infra/repository/note_project_repository.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::infra::repository
{
int64_t NoteProjectRepository::Create(const silok::domain::NoteProject& relation)
{
    return infra::StorageManager::Insert(relation);
}

std::optional<silok::domain::NoteProject> NoteProjectRepository::FindById(int64_t id)
{
    auto relations = infra::StorageManager::FindByField<silok::domain::NoteProject>(
        &silok::domain::NoteProject::id, id);
    if (relations.empty())
    {
        return std::nullopt;
    }
    return relations.front();
}

std::vector<silok::domain::NoteProject> NoteProjectRepository::FindByFirstId(int64_t first_id)
{
    return infra::StorageManager::FindByField<silok::domain::NoteProject>(
        &silok::domain::NoteProject::first_id, first_id);
}

std::vector<silok::domain::NoteProject> NoteProjectRepository::FindBySecondId(int64_t second_id)
{
    return infra::StorageManager::FindByField<silok::domain::NoteProject>(
        &silok::domain::NoteProject::second_id, second_id);
}

bool NoteProjectRepository::Update(const silok::domain::NoteProject& relation)
{
    return infra::StorageManager::Update(relation);
}

void NoteProjectRepository::Delete(int64_t id)
{
    silok::domain::NoteProject relation =
        infra::StorageManager::FindByField<silok::domain::NoteProject>(
            &silok::domain::NoteProject::id, id)
            .front();
    infra::StorageManager::Remove(relation);
}

}  // namespace silok::infra::repository
