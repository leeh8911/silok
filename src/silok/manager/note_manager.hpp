#pragma once

#include <string>

#include "silok/model.hpp"

namespace silok::manager
{
class NoteManager
{
 public:
    NoteManager() = default;

    void CreateNote(const std::string& content, const User& user);
};
}  // namespace silok::manager
