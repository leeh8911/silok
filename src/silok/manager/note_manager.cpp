#include "silok/manager/note_manager.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

#include "silok/manager/storage_manager.hpp"
#include "silok/manager/utils.hpp"
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

void NoteManager::UpdateNote(Note note, const User user)
{
    if (!utils::HasPermission<UserNote>(user, note, "owner"))
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

void NoteManager::DeleteNote(Note note, const User user)
{
    if (!utils::HasPermission<UserNote>(user, note, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to update note ID: {}", user.email,
                        note.id);
        return;
    }

    SILOK_LOG_INFO("User {} is deleting note '{}...'", user.email, note.content.substr(0, 20));
    StorageManager::Remove(note);
}

void NoteManager::LinkNoteToTag(Note note, Tag tag, const User user)
{
    if (!utils::HasPermission<UserTag>(user, tag, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to link note ID: {} with tag ID: {}",
                        user.email, note.id, tag.id);
        return;
    }

    NoteTag note_tag;
    note_tag.note_id = note.id;
    note_tag.tag_id = tag.id;

    StorageManager::Insert(note_tag);
}

void NoteManager::UnlinkNoteFromTag(Note note, Tag tag, const User user)
{
    if (!utils::HasPermission<UserTag>(user, tag, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to unlink note ID: {} from tag ID: {}",
                        user.email, note.id, tag.id);
        return;
    }

    auto note_tags =
        StorageManager::FindByFields<NoteTag>(&NoteTag::note_id, note.id, &NoteTag::tag_id, tag.id);

    if (note_tags.empty())
    {
        SILOK_LOG_WARN("No link found between note ID: {} and tag ID: {}", note.id, tag.id);
        return;
    }

    StorageManager::Remove(note_tags.front());
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

std::vector<Note> NoteManager::GetAllNotesByTag(const User user, const Tag tag) const
{
    if (!utils::HasPermission<UserTag>(user, tag, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to access tag ID: {}", user.email,
                        tag.id);
        return {};
    }

    auto note_tags = StorageManager::FindByField<NoteTag>(&NoteTag::tag_id, tag.id);
    std::vector<int64_t> note_ids{};
    std::transform(note_tags.begin(), note_tags.end(), std::back_inserter(note_ids),
                   [](const NoteTag& nt) { return nt.note_id; });

    std::vector<Note> notes = StorageManager::FindByFieldIn<Note>(&Note::id, note_ids);
    return notes;
}

}  // namespace silok::manager
