#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace silok::domain
{
struct Note
{
    int64_t id;
    std::string content;
    int64_t created_at;
    int64_t updated_at;
};

}