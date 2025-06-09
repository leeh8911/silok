#include <gtest/gtest.h>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/db/sql/sqlite_db_statement.hpp"
#include "silok/db/sql/sqlite_schema_manager.hpp"
#include "silok/repository/tag_repository.hpp"
#include "silok/repository/user_repository.hpp"

class TagRepositoryTest : public ::testing::Test
{
 protected:
    silok::db::SqliteDBConnectionPtr db{nullptr};
    silok::repository::TagRepository tag_repo;
    silok::repository::UserRepository user_repo;

    void SetUp() override
    {
        // Setup code for the test
        db = std::make_shared<silok::db::SqliteDBConnection>(":memory:");

        silok::db::SqliteSchemaManager schemaManager;
        schemaManager.migrate(*db);

        tag_repo = silok::repository::TagRepository(db);
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST_F(TagRepositoryTest, BasicTagCRUDScenario)
{
    user_repo.createUser("testuser");
    auto user = user_repo.getUserByName("testuser").value();

    tag_repo.createTag("testtag", user.id);
    auto tags = tag_repo.getTagsByOwner(user.id).value();
    EXPECT_EQ(tags.size(), 1);

    auto tag = tags[0];
    EXPECT_GT(tag.id, 0);
    EXPECT_EQ(tag.name, "testtag");

    auto found = tag_repo.getTagById(tag.id, user.id).value();
    EXPECT_EQ(found.id, tag.id);
    EXPECT_EQ(found.name, "testtag");

    tag_repo.deleteTag(tag.id, user.id);
    EXPECT_FALSE(tag_repo.getTagById(tag.id, user.id).has_value());
}

TEST_F(TagRepositoryTest, WrongOwnershipScenario)
{
    user_repo.createUser("owner");
    user_repo.createUser("shared_user");
    auto owner = user_repo.getUserByName("owner").value();
    auto shared_user = user_repo.getUserByName("shared_user").value();

    tag_repo.createTag("shared_tag", owner.id);
    auto tags = tag_repo.getTagsByOwner(owner.id).value();
    EXPECT_EQ(tags.size(), 1);

    auto tag = tags[0];

    auto owner_found = tag_repo.getTagById(tag.id, owner.id).value();
    EXPECT_EQ(owner_found.id, tag.id);
    EXPECT_EQ(owner_found.name, "shared_tag");

    EXPECT_FALSE(tag_repo.getTagById(tag.id, shared_user.id)
                     .has_value());  // Should not be found since shared_user has no access

    tag_repo.shareTag(tag.id, owner.id, shared_user.id);
    auto shared_found = tag_repo.getTagById(tag.id, shared_user.id).value();
    EXPECT_EQ(shared_found.id, tag.id);

    try
    {
        tag_repo.deleteTag(tag.id, shared_user.id);
        FAIL() << "Expected an exception when shared user tries to delete tag";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "User does not have permission to delete this tag");
    }
}
