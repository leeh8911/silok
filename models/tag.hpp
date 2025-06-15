#pragma once

#include <string>

#pragma db object
class Tag
{
 public:
#pragma db id auto
    unsigned long id;
    std::string name;
    Tag() = default;
    Tag(const std::string& name) : name(name) {}
};
