#include "silok/manager/note_manager.hpp"

#include <chrono>

#include "silok/manager/storage_manager.hpp"
#include "silok/model.hpp"

namespace silok::manager
{
void NoteManager::CreateNote(const std::string& content, const User& user)
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
}  // namespace silok::manager
