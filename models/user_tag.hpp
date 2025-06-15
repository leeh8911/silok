#pragma once

#include <string>

#pragma db value
struct UserTagId
{
    unsigned long user_id;
    unsigned long tag_id;

    UserTagId() = default;
    UserTagId(unsigned long uid, unsigned long tid) : user_id(uid), tag_id(tid) {}

    bool operator==(const UserTagId& other) const = default;
};

#pragma db object
class UserTag
{
 public:
#pragma db id
    UserTagId id;
    std::string role;
    UserTag() = default;
    UserTag(unsigned long uid, unsigned long tid, const std::string& role)
        : id{uid, tid}, role(role)
    {
    }
};
