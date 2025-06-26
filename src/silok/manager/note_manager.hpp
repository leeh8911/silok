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
    std::vector<Note> GetAllNotes(const User user) const;
    void UpdateNote(Note note, const User user);
    void DeleteNote(Note note, const User user);

 private:
};
}  // namespace silok::manager
