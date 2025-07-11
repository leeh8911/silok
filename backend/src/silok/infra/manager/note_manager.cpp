#include "note_manager.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/infra/manager/utils.hpp"

namespace silok::infra
{
void NoteManager::CreateNote(const User user, const std::string& content)
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

void NoteManager::UpdateNote(const User user, Note note)
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

void NoteManager::DeleteNote(const User user, Note note)
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

void NoteManager::LinkNoteToTag(const User user, Note note, Tag tag)
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

void NoteManager::UnlinkNoteFromTag(const User user, Note note, Tag tag)
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

void NoteManager::LinkNoteToProject(const User user, Note note, Project project)
{
    if (!utils::HasPermission<UserProject>(user, project, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to link note ID: {} with project ID: {}",
                        user.email, note.id, project.id);
        return;
    }

    NoteProject note_project;
    note_project.note_id = note.id;
    note_project.project_id = project.id;

    StorageManager::Insert(note_project);
}
void NoteManager::UnlinkNoteFromProject(const User user, Note note, Project project)
{
    if (!utils::HasPermission<UserProject>(user, project, "owner"))
    {
        SILOK_LOG_ERROR(
            "User {} does not have permission to unlink note ID: {} from project ID: {}",
            user.email, note.id, project.id);
        return;
    }

    auto note_projects = StorageManager::FindByFields<NoteProject>(
        &NoteProject::note_id, note.id, &NoteProject::project_id, project.id);

    if (note_projects.empty())
    {
        SILOK_LOG_WARN("No link found between note ID: {} and project ID: {}", note.id, project.id);
        return;
    }

    StorageManager::Remove(note_projects.front());
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
std::vector<Note> NoteManager::GetAllNotesByProject(const User user, const Project project) const
{
    if (!utils::HasPermission<UserProject>(user, project, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to access project ID: {}", user.email,
                        project.id);
        return {};
    }

    auto note_projects =
        StorageManager::FindByField<NoteProject>(&NoteProject::project_id, project.id);
    std::vector<int64_t> note_ids{};
    std::transform(note_projects.begin(), note_projects.end(), std::back_inserter(note_ids),
                   [](const NoteProject& np) { return np.note_id; });

    std::vector<Note> notes = StorageManager::FindByFieldIn<Note>(&Note::id, note_ids);
    return notes;
}

}  // namespace silok::infra
