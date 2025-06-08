#pragma once

#include <chrono>
#include <string>

namespace silok
{

enum class NoteType
{
    Memo,
    Task,
    Event
};

using Id = uint64_t;

class Tag;
class Project;

struct Note
{
    uint64_t id{};
    NoteType type{};
    std::string content{};
    std::chrono::system_clock::time_point created_at{};
    std::chrono::system_clock::time_point updated_at{};

    std::optional<std::chrono::system_clock::time_point> start_date{};
    std::optional<std::chrono::system_clock::time_point> end_date{};
    std::optional<bool> done{};
};

using NoteList = std::vector<Note>;

}  // namespace silok
