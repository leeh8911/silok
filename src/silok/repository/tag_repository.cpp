#include "silok/repository/tag_repository.hpp"

#include "silok/db/base/base_db_connection.hpp"
#include "silok/db/base/base_db_statement.hpp"
#include "silok/logger.hpp"

namespace silok::repository
{

TagRepository::TagRepository(silok::db::BaseDBConnectionPtr db_) : db(std::move(db_)) {}

void TagRepository::createTag(const std::string& name, uint64_t owner_id)
{
    auto stmt = db->prepare("INSERT INTO tags(name) VALUES(?);");
    stmt->bind(1, name);
    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to create tag: " + name);
    }

    uint64_t tag_id = db->lastInsertId();
    shareTag(tag_id, owner_id, owner_id);
}

std::vector<Tag> TagRepository::getTagsByOwner(uint64_t owner_id)
{
    auto stmt = db->prepare(R"(
        SELECT t.id, t.name, ut.role
        FROM tags t
        JOIN user_tag ut ON t.id = ut.tag_id
        WHERE ut.user_id = ?
    )");
    stmt->bind(1, owner_id);

    std::vector<Tag> tags{};
    while (stmt->step() == silok::db::StepResult::Row)
    {
        Tag tag;
        tag.id = stmt->columnInt(0);
        tag.name = stmt->columnText(1);
        tag.role = Role::fromString(stmt->columnText(2));
        tags.push_back(tag);
    }
    return tags;
}

std::optional<Tag> TagRepository::getTagById(uint64_t id, uint64_t user_id)
{
    auto stmt = db->prepare(R"(
        SELECT t.id, t.name, ut.role
        FROM tags t
        JOIN user_tag ut ON t.id = ut.tag_id
        WHERE t.id = ? AND ut.user_id = ?
    )");
    stmt->bind(1, id);
    stmt->bind(2, user_id);

    if (stmt->step() == silok::db::StepResult::Row)
    {
        Tag tag;
        tag.id = stmt->columnInt(0);
        tag.name = stmt->columnText(1);
        tag.role = Role::fromString(stmt->columnText(2));
        return tag;
    }
    return std::nullopt;
}

void TagRepository::deleteTag(uint64_t id, uint64_t owner_id)
{
    if (!isTagOwned(id, owner_id))
    {
        throw std::runtime_error("User does not have permission to delete this tag");
    }
    if (!getTagById(id, owner_id).has_value())
    {
        throw std::runtime_error("Tag not found or does not belong to the user");
    }

    auto stmt = db->prepare("DELETE FROM tags WHERE id = ?;");
    stmt->bind(1, id);
    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to delete tag with ID: " + std::to_string(id));
    }
}

void TagRepository::shareTag(uint64_t tag_id, uint64_t owner_id, uint64_t shared_user_id)
{
    std::string role = (owner_id == shared_user_id) ? "owner" : "shared";

    auto stmt = db->prepare("INSERT INTO user_tag(user_id, tag_id, role) VALUES(?, ?, ?);");
    stmt->bind(1, shared_user_id);
    stmt->bind(2, tag_id);
    stmt->bind(3, role);

    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to share tag with user ID: " +
                                 std::to_string(shared_user_id));
    }
}

bool TagRepository::isTagOwned(uint64_t tag_id, uint64_t user_id) const
{
    auto stmt = db->prepare(R"(
        SELECT COUNT(*) FROM user_tag
        WHERE tag_id = ? AND user_id = ? AND role = 'owner'
    )");
    stmt->bind(1, tag_id);
    stmt->bind(2, user_id);
    return stmt->step() == silok::db::StepResult::Row && stmt->columnInt(0) > 0;
}

bool TagRepository::isTagShared(uint64_t tag_id, uint64_t user_id) const
{
    auto stmt = db->prepare(R"(
        SELECT COUNT(*) FROM user_tag
        WHERE tag_id = ? AND user_id = ? AND role = 'shared'
    )");
    stmt->bind(1, tag_id);
    stmt->bind(2, user_id);
    return stmt->step() == silok::db::StepResult::Row && stmt->columnInt(0) > 0;
}

}  // namespace silok::repository
