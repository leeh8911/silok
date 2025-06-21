#include <gtest/gtest.h>

#include "../src/silok/domain/model.hpp"
#include "../src/silok/domain/model_relation.hpp"
#include "silok/application/user_service.hpp"
#include "silok/infra/repository/user_repository.hpp"
#include "silok/infra/storage_manager.hpp"

class TestUserService : public ::testing::Test
{
 protected:
    // You can define per-test set-up logic as needed.
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);

        user_service = silok::application::UserService(
            std::make_shared<silok::infra::repository::UserRepository>());
    }

    // You can define per-test tear-down logic as needed.
    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }
    silok::application::UserService user_service;
};

TEST_F(TestUserService, UserService_CreateAndFindUser)
{
    user_service.Create("john.doe", "john.doe@test.com", "password");

    auto user = user_service.FindByEmail("john.doe@test.com");

    ASSERT_TRUE(user.has_value());
    ASSERT_EQ(user->name, "john.doe");
    ASSERT_EQ(user->email, "john.doe@test.com");
    ASSERT_NE(user->password, "password");

    ASSERT_EQ(user_service.Login("john.doe@test.com", "wrong_password"), std::nullopt);
    ASSERT_EQ(user_service.Login("wrong_email@test.com", "password"), std::nullopt);

    auto token = user_service.Login("john.doe@test.com", "password");
    ASSERT_TRUE(token.has_value());
}
TEST_F(TestUserService, UserService_DuplicateEmailShouldFail)
{
    user_service.Create("john.doe", "john.doe@test.com", "password");

    // 예외를 던지도록 구현되었다면:
    EXPECT_THROW(
        { user_service.Create("duplicate", "john.doe@test.com", "newpassword"); },
        std::runtime_error);
}
TEST_F(TestUserService, UserService_UpdateUser)
{
    user_service.Create("alice", "alice@test.com", "password");

    auto user = user_service.FindByEmail("alice@test.com");
    ASSERT_TRUE(user.has_value());

    user->name = "Alice Smith";
    user_service.Update(*user);

    auto updated_user = user_service.FindByEmail("alice@test.com");
    ASSERT_TRUE(updated_user.has_value());
    EXPECT_EQ(updated_user->name, "Alice Smith");
}
TEST_F(TestUserService, UserService_DeleteUser)
{
    user_service.Create("bob", "bob@test.com", "password");

    auto user = user_service.FindByEmail("bob@test.com");
    ASSERT_TRUE(user.has_value());

    user_service.Delete(*user);

    auto deleted = user_service.FindByEmail("bob@test.com");
    EXPECT_FALSE(deleted.has_value());

    auto token = user_service.Login("bob@test.com", "password");
    EXPECT_FALSE(token.has_value());
}
TEST_F(TestUserService, UserService_PasswordUpdate)
{
    user_service.Create("changepw", "pw@test.com", "original");

    auto user = user_service.FindByEmail("pw@test.com");
    ASSERT_TRUE(user.has_value());

    user->password = "new_password";  // 암호화는 Update 내부에서 처리된다고 가정
    user_service.Update(*user);

    EXPECT_FALSE(user_service.Login("pw@test.com", "original").has_value());
    EXPECT_TRUE(user_service.Login("pw@test.com", "new_password").has_value());
}
