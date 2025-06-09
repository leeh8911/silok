#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "silok/data/tag.hpp"
#include "silok/db/base/base_db_connection.hpp"

namespace silok::repository
{
class TagRepository
{
 public:
    explicit TagRepository(silok::db::BaseDBConnectionPtr db_);

    void createTag(const std::string& name, uint64_t owner_id);

    std::optional<std::vector<Tag>> getTagsByOwner(uint64_t owner_id);

    std::optional<Tag> getTagById(uint64_t id, uint64_t owner_id);

    void deleteTag(uint64_t id, uint64_t owner_id);

    void shareTag(uint64_t tag_id, uint64_t owner_id, uint64_t shared_user_id);

 private:
    silok::db::BaseDBConnectionPtr db{nullptr};
};

}  // namespace silok::repository
