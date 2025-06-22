#include "silok/infra/repository/note_repository.hpp"

#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

namespace silok::infra::repository
{

using silok::domain::Note;
using silok::infra::StorageManager;

int64_t NoteRepository::Create(const Note& note)
{
    SILOK_LOG_INFO("Creating note with content: {}", note.content);
    return StorageManager::Insert(note);
}

std::optional<Note> NoteRepository::FindById(int64_t id)
{
    SILOK_LOG_INFO("Finding note by ID: {}", id);
    auto notes = StorageManager::FindByField<Note>(&Note::id, id);
    if (notes.empty())
    {
        return std::nullopt;
    }
    return notes.front();
}

std::vector<Note> NoteRepository::FindByIds(const std::vector<int64_t>& ids)
{
    SILOK_LOG_INFO("Finding notes by IDs");
    return StorageManager::FindByFieldIn<Note>(&Note::id, ids);
}

bool NoteRepository::Update(const Note& note)
{
    try
    {
        StorageManager::Update(note);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void NoteRepository::Delete(int64_t id)
{
    SILOK_LOG_INFO("Deleting note with ID: {}", id);
    Note note = StorageManager::FindByField<Note>(&Note::id, id).front();
    StorageManager::Remove(note);
}

}  // namespace silok::infra::repository
