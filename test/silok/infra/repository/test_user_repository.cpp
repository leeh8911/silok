#include <gtest/gtest.h>

#include "silok/domain/model.hpp"
#include "silok/infra/repository/user_repository.hpp"
#include "silok/infra/storage_manager.hpp"
#include "silok/logger.hpp"

class UserRepositoryTest : public ::testing::Test
{
 protected:
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST_F(UserRepositoryTest, CreateUser)
{
    silok::infra::repository::UserRepository user_repository;

    silok::domain::User user;
    user.name = "test_user";
    user.email = "user@test.com";
    user.password = "securepassword";

    auto user_id = user_repository.Create(user);
    ASSERT_GT(user_id, 0);
}

TEST_F(UserRepositoryTest, FindUpdateAndDelete)
{
    silok::infra::repository::UserRepository repo;
    silok::domain::User user{0, "name", "mail@test", "pw"};
    auto id = repo.Create(user);

    auto found = repo.FindByEmail("mail@test");
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->id, id);

    found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());

    user.id = id;
    user.name = "changed";
    EXPECT_TRUE(repo.Update(user));
    found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->name, "changed");

    repo.Delete(id);
    EXPECT_FALSE(repo.FindById(id).has_value());
}
