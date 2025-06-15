#pragma once

#include <string>

#pragma db value
struct NoteTagId
{
    unsigned long note_id;
    unsigned long tag_id;

    NoteTagId() = default;
    NoteTagId(unsigned long nid, unsigned long tid) : note_id(nid), tag_id(tid) {}

    bool operator==(const NoteTagId& other) const = default;
};

#pragma db object
class NoteTag
{
 public:
#pragma db id
    NoteTagId id;
    NoteTag() = default;
    NoteTag(unsigned long nid, unsigned long tid) : id{nid, tid} {}
};
