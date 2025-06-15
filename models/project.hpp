#pragma once

#include <string>

#pragma db object
class Project
{
 public:
#pragma db id auto
    unsigned long id;
    std::string name;
    Project() = default;
    Project(const std::string& name) : name(name) {}
};
