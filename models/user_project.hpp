#pragma once

#include <string>

#pragma db value
struct UserProjectId
{
    unsigned long user_id;
    unsigned long project_id;

    UserProjectId() = default;
    UserProjectId(unsigned long uid, unsigned long pid) : user_id(uid), project_id(pid) {}

    bool operator==(const UserProjectId& other) const = default;
};

#pragma db object
class UserProject
{
 public:
#pragma db id
    UserProjectId id;
    std::string role;
    UserProject() = default;
    UserProject(unsigned long uid, unsigned long pid, const std::string& role)
        : id{uid, pid}, role(role)
    {
    }
};
