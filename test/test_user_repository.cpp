#include <gtest/gtest.h>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/repository/user_repository.hpp"

class UserRepositoryTest : public ::testing::Test
{
 protected:
    ::silok::repository::UserRepository userRepo;
    ::silok::db::SqliteDBConnectionPtr db{nullptr};

    void SetUp() override
    {
        // Setup code if needed
        db = std::make_shared<silok::db::SqliteDBConnection>(":memory:");

        ::silok::db::SqliteSchemaManager schemaManager;
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
    auto user = userRepo.createUser("testuser");
    EXPECT_GT(user.id, 0);
    EXPECT_EQ(user.username, "testuser");

    // Verify user exists in the database
    auto found = userRepo.getUserById(user.id);
    EXPECT_EQ(found.id, user.id);
    EXPECT_EQ(found.username, "testuser");

    // Verify user is deleted
    userRepo.deleteUser(user.id);
    auto deleted = userRepo.getUserById(user.id);
    EXPECT_FALSE(deleted.has_value());
}

TEST_F(UserRepositoryTest, DuplicatedUserCreation)
{
    // Create a user
    auto user1 = userRepo.createUser("duplicateuser");

    // Attempt to create the same user again
    try
    {
        auto user2 = userRepo.createUser("duplicateuser");
        FAIL() << "Expected an exception for duplicate username";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "User with username 'duplicateuser' already exists");
    }
}