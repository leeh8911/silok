#pragma once

#include <string>

#pragma db object
class Note
{
 public:
#pragma db id auto
    unsigned long id;
    std::string content;
    unsigned long created_at;
    unsigned long updated_at;
    Note() = default;
    Note(const std::string& content, unsigned long created, unsigned long updated)
        : content(content), created_at(created), updated_at(updated)
    {
    }
};
