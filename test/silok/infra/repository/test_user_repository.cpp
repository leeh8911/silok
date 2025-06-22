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
