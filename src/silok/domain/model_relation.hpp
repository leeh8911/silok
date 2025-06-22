#pragma once

#include <cstdint>

namespace silok::domain
{
struct UserNote
{
    int64_t id;
    int64_t first_id;   // 사용자 ID
    int64_t second_id;  // 노트 ID
};
struct UserTag
{
    int64_t id;         // ID
    int64_t first_id;   // 사용자 ID
    int64_t second_id;  // 태그 ID
};
struct UserProject
{
    int64_t id;         // ID
    int64_t first_id;   // 사용자 ID
    int64_t second_id;  // 프로젝트 ID
};

struct NoteTag
{
    int64_t id;         // ID
    int64_t first_id;   // 노트 ID
    int64_t second_id;  // 태그 ID
};
struct NoteProject
{
    int64_t id;         // ID
    int64_t first_id;   // 노트 ID
    int64_t second_id;  // 프로젝트 ID
};

}  // namespace silok::domain
