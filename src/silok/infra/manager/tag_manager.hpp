#pragma once

#include <vector>

#include "silok/domain/model.hpp"

namespace silok::infra
{
class TagManager
{
 public:
    void CreateTag(const User user, const std::string& tag_name);
    std::vector<Tag> GetAllTags(const User user) const;
    void UpdateTag(const User user, Tag tag);
    void DeleteTag(const User user, Tag tag);

 private:
};
}  // namespace silok::infra
