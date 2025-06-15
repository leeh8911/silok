#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace silok::data
{

enum class Role
{
    Owner,
    Shared
};

struct User
{
    uint64_t id;
    std::string username;
};
struct Project
{
    uint64_t id;
    std::string name;
};
struct Tag
{
    uint64_t id;
    std::string name;
};
struct Note
{
    uint64_t id;
    std::string content;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
};

struct UserProjectRelation
{
    uint64_t user_id;
    uint64_t project_id;
    Role role;
};

struct UserTagRelation
{
    uint64_t user_id;
    uint64_t tag_id;
    Role role;
};

struct UserNoteRelation
{
    uint64_t user_id;
    uint64_t note_id;
    Role role;
};
struct NoteProjectRelation
{
    uint64_t note_id;
    uint64_t project_id;
};

struct NoteTagRelation
{
    uint64_t note_id;
    uint64_t tag_id;
};

}  // namespace silok::data
