#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace silok::domain
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
    int64_t id;        // ID
    std::string name;  // 태그 내용
};
struct Project
{
    int64_t id;        // ID
    std::string name;  //
    int64_t start;
    int64_t end;
};
}  // namespace silok::domain
