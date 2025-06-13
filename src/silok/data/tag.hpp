#pragma once

#include <cstdint>
#include <string>

#include "silok/data/common.hpp"

namespace silok
{
class Tag
{
 public:
    uint64_t id{};
    std::string name{};
    Role role{};
};
}  // namespace silok
