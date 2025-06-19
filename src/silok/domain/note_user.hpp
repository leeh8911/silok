#pragma once

#include <cstdint>
#include <string>

namespace silok::domain
{
struct NoteUser
{
    int64_t id;
    int64_t user_id;  // 사용자 ID
    int64_t note_id;  // 노트 ID
};
}  // namespace silok::domain