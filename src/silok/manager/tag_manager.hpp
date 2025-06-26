#pragma once

#include "silok/model.hpp"

#include <vector>

namespace silok::manager
{
class TagManager
{
 public:
    void CreateTag(const std::string& tag_name, const User user);
    std::vector<Tag> GetAllTags(const User user) const;
    void UpdateTag(Tag tag, const User user);
    void DeleteTag(Tag tag, const User user);

 private:
};
}  // namespace silok::manager
