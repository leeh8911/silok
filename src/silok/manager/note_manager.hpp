#pragma once

#include <string>
#include <vector>

#include "silok/model.hpp"

namespace silok::manager
{
class NoteManager
{
 public:
    NoteManager() = default;

    void CreateNote(const std::string& content, const User user);

    void UpdateNote(Note note, const User user);
    void DeleteNote(Note note, const User user);

    void LinkNoteToTag(Note note, Tag tag, const User user);
    void UnlinkNoteFromTag(Note note, Tag tag, const User user);

    std::vector<Note> GetAllNotes(const User user) const;
    std::vector<Note> GetAllNotesByTag(const User user, const Tag tag) const;

 private:
};
}  // namespace silok::manager
