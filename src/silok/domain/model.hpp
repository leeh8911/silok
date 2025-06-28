#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace silok
{
struct User
{
    int64_t id;
    std::string name;
    std::string email;
    std::string password;
};

struct Note
{
    int64_t id;
    std::string content;
    int64_t created_at;
    int64_t updated_at;
};

struct Tag
{
    int64_t id;
    std::string name;
};

struct Project
{
    int64_t id;
    std::string name;
    std::optional<int64_t> start;
    std::optional<int64_t> end;
};

struct UserNote
{
    int64_t id;
    int64_t user_id;
    int64_t note_id;
    std::string role;
};

struct UserTag
{
    int64_t id;
    int64_t user_id;
    int64_t tag_id;
    std::string role;
};

struct UserProject
{
    int64_t id;
    int64_t user_id;
    int64_t project_id;
    std::string role;
};

struct NoteTag
{
    int64_t id;
    int64_t note_id;
    int64_t tag_id;
};

struct NoteProject
{
    int64_t id;
    int64_t note_id;
    int64_t project_id;
};

}  // namespace silok
