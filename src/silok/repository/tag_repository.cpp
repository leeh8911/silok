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

    // Get the last inserted ID
    uint64_t tag_id = db->lastInsertId();

    // Associate the tag with the owner
    shareTag(tag_id, owner_id, owner_id);
}

std::optional<std::vector<Tag>> TagRepository::getTagsByOwner(uint64_t owner_id)
{
    auto stmt = db->prepare(
        "SELECT id, name FROM tags WHERE id IN "
        "(SELECT tag_id FROM user_tag WHERE user_id = ?);");
    stmt->bind(1, owner_id);

    std::vector<Tag> tags;
    while (stmt->step() == silok::db::StepResult::Row)
    {
        Tag tag;
        tag.id = stmt->columnInt(0);
        tag.name = stmt->columnText(1);
        tags.push_back(tag);
    }

    if (tags.empty())
    {
        return std::nullopt;  // No tags found for the owner
    }
    return tags;
}

std::optional<Tag> TagRepository::getTagById(uint64_t id, uint64_t owner_id)
{
    auto stmt = db->prepare("SELECT id, name FROM tags WHERE id = ?;");
    stmt->bind(1, id);
    if (stmt->step() == silok::db::StepResult::Row)
    {
        Tag tag;
        tag.id = stmt->columnInt(0);
        tag.name = stmt->columnText(1);
        return tag;
    }
    return std::nullopt;  // Tag not found
}

void TagRepository::deleteTag(uint64_t id, uint64_t owner_id)
{
    // Check if the tag exists and belongs to the owner
    auto tag = getTagById(id, owner_id);
    if (!tag.has_value())
    {
        throw std::runtime_error("Tag not found or does not belong to the user");
    }

    // Delete the tag
    auto stmt = db->prepare("DELETE FROM tags WHERE id = ?;");
    stmt->bind(1, id);
    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to delete tag with ID: " + std::to_string(id));
    }
}

void TagRepository::shareTag(uint64_t tag_id, uint64_t owner_id, uint64_t shared_user_id)
{
    // Check if the tag exists and belongs to the owner
    auto tag = getTagById(tag_id, owner_id);
    if (!tag.has_value())
    {
        throw std::runtime_error("Tag not found or does not belong to the user");
    }

    // Share the tag with another user
    auto stmt = db->prepare("INSERT INTO user_tag(user_id, tag_id, role) VALUES(?, ?, 'shared');");
    stmt->bind(1, shared_user_id);
    stmt->bind(2, tag_id);
    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to share tag with user ID: " +
                                 std::to_string(shared_user_id));
    }
}

}  // namespace silok::repository
