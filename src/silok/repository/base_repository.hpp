#pragma once

#include "silok/note.hpp"

namespace silok::repository
{
class BaseRepository
{
 public:
    BaseRepository() = default;

    virtual ~BaseRepository() = default;
    virtual uint64_t createNote() = 0;
    virtual ::silok::Note getNote(uint64_t id) = 0;
    virtual bool updateNote(const Note& note) = 0;
    virtual bool deleteNote(uint64_t id) = 0;
};
}  // namespace silok::repository
