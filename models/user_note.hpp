#pragma once

#include <string>

#pragma db value
struct UserNoteId
{
    unsigned long user_id;
    unsigned long note_id;

    UserNoteId() = default;
    UserNoteId(unsigned long uid, unsigned long nid) : user_id(uid), note_id(nid) {}

    bool operator==(const UserNoteId& other) const = default;
};

#pragma db object
class UserNote
{
 public:
#pragma db id
    UserNoteId id;     // Composite key of user_id and note_id
    std::string role;  // "owner" or "shared"

    UserNote() = default;
    UserNote(unsigned long uid, unsigned long nid, const std::string& role)
        : id(uid, nid), role(role)
    {
    }
};