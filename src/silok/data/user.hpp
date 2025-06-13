#pragma once

#include <cstdint>
#include <string>

namespace silok
{
class User
{
 public:
    uint64_t id{};
    std::string username{};
};

}  // namespace silok