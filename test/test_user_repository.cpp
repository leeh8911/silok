#include <gtest/gtest.h>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/db/sql/sqlite_db_statement.hpp"
#include "silok/db/sql/sqlite_schema_manager.hpp"
#include "silok/repository/user_repository.hpp"

class UserRepositoryTest : public ::testing::Test
{
 protected:
    silok::repository::UserRepository user_repo;
    silok::db::SqliteDBConnectionPtr db{nullptr};

    void SetUp() override
    {
        // Setup code if needed
        db = std::make_shared<silok::db::SqliteDBConnection>(":memory:");

        silok::db::SqliteSchemaManager schemaManager;
        schemaManager.migrate(*db);

        user_repo = silok::repository::UserRepository(db);
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST_F(UserRepositoryTest, BasicUserCRUDScenario)
{
    // Test creating a user
    user_repo.createUser("testuser");

    auto user = user_repo.getUserByName("testuser").value();
    EXPECT_GT(user.id, 0);
    EXPECT_EQ(user.username, "testuser");

    // Verify user exists in the database
    auto found = user_repo.getUserById(user.id).value();
    EXPECT_EQ(found.id, user.id);
    EXPECT_EQ(found.username, "testuser");

    // Verify user is deleted
    user_repo.deleteUser(user.id);
    EXPECT_FALSE(user_repo.getUserById(user.id).has_value());
}

TEST_F(UserRepositoryTest, DuplicatedUserCreation)
{
    // Create a user
    user_repo.createUser("duplicateuser");
    auto user1 = user_repo.getUserByName("duplicateuser").value();

    // Attempt to create the same user again
    try
    {
        user_repo.createUser("duplicateuser");
        FAIL() << "Expected an exception for duplicate username";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "User with username 'duplicateuser' already exists");
    }
}
