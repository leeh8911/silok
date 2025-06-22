#include "silok/infra/repository/note_repository.hpp"

#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

namespace silok::infra::repository
{

int64_t NoteRepository::Create(const silok::domain::Note& note)
{
    SILOK_LOG_INFO("Creating note with content: {}", note.content);
    return infra::StorageManager::Insert(note);
}

std::optional<silok::domain::Note> NoteRepository::FindById(int64_t id)
{
    SILOK_LOG_INFO("Finding note by ID: {}", id);
    auto notes =
        infra::StorageManager::FindByField<silok::domain::Note>(&silok::domain::Note::id, id);
    if (notes.empty())
    {
        return std::nullopt;
    }
    return notes.front();
}

std::vector<silok::domain::Note> NoteRepository::FindByIds(const std::vector<int64_t>& ids)
{
    SILOK_LOG_INFO("Finding notes by IDs");
    return infra::StorageManager::FindByFieldIn<silok::domain::Note>(&silok::domain::Note::id, ids);
}

void NoteRepository::Update(const silok::domain::Note& note)
{
    SILOK_LOG_INFO("Updating note with ID: {}", note.id);
    infra::StorageManager::Update(note);
}

void NoteRepository::Delete(int64_t id)
{
    SILOK_LOG_INFO("Deleting note with ID: {}", id);
    silok::domain::Note note =
        infra::StorageManager::FindByField<silok::domain::Note>(&silok::domain::Note::id, id)
            .front();
    infra::StorageManager::Remove(note);
}

}  // namespace silok::infra::repository
