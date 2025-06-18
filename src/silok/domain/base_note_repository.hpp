#pragma once

#include <cstdint>
#include <vector>

#include "silok/domain/note.hpp"

namespace silok::domain
{
class BaseNoteRepository
{
 public:
    virtual ~BaseNoteRepository() = default;

    virtual void create(const Note& note) = 0;
    virtual Note read(int64_t id) const = 0;
    virtual void update(const Note& note) = 0;
    virtual void remove(int64_t id) = 0;
    virtual std::vector<Note> list() const = 0;
};
}