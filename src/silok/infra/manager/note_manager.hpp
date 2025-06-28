#pragma once

#include <string>
#include <vector>

#include "silok/domain/model.hpp"

namespace silok::infra
{
class NoteManager
{
 public:
    NoteManager() = default;

    void CreateNote(const User user, const std::string& content);

    void UpdateNote(const User user, Note note);
    void DeleteNote(const User user, Note note);

    void LinkNoteToTag(const User user, Note note, Tag tag);
    void UnlinkNoteFromTag(const User user, Note note, Tag tag);

    std::vector<Note> GetAllNotes(const User user) const;
    std::vector<Note> GetAllNotesByTag(const User user, const Tag tag) const;

 private:
};
}  // namespace silok::infra
