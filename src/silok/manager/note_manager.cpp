#include "silok/manager/note_manager.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

#include "silok/manager/storage_manager.hpp"
#include "silok/model.hpp"

namespace silok::manager
{
void NoteManager::CreateNote(const std::string& content, const User user)
{
    Note note;
    note.content = content;
    note.created_at = std::chrono::system_clock::now().time_since_epoch().count();
    note.updated_at = note.created_at;

    auto note_id = StorageManager::Insert(note);

    UserNote user_note;
    user_note.user_id = user.id;
    user_note.note_id = note_id;
    user_note.role = "owner";  // Default role for the creator

    StorageManager::Insert(user_note);
}

std::vector<Note> NoteManager::GetAllNotes(const User user) const
{
    auto user_notes = StorageManager::FindByField<UserNote>(&UserNote::user_id, user.id);
    std::vector<int64_t> note_ids{};
    std::transform(user_notes.begin(), user_notes.end(), std::back_inserter(note_ids),
                   [](const UserNote& un) { return un.note_id; });

    std::vector<Note> notes = StorageManager::FindByFieldIn<Note>(&Note::id, note_ids);
    return notes;
}

void NoteManager::UpdateNote(Note note, const User user)
{
    auto user_notes = StorageManager::FindByField<UserNote>(&UserNote::note_id, note.id);
    if (user_notes.empty())
    {
        SILOK_LOG_ERROR("No user notes found for note ID: {}", note.id);
        return;
    }
    if (std::none_of(user_notes.begin(), user_notes.end(), [&user](const UserNote& un)
                     { return ((un.user_id == user.id) && (un.role == "owner")); }))
    {
        SILOK_LOG_ERROR("User {} does not have permission to update note ID: {}", user.email,
                        note.id);
        return;
    }

    note.updated_at =
        std::chrono::system_clock::now().time_since_epoch().count();  // Update timestamp

    SILOK_LOG_INFO("User {} is updating note '{}...'", user.email, note.content.substr(0, 20));
    StorageManager::Update(note);
}
}  // namespace silok::manager
