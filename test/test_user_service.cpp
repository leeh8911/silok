#include <gtest/gtest.h>

#include "silok/application/user_service.hpp"
#include "silok/domain/data.hpp"
#include "silok/domain/data_relation.hpp"
#include "silok/infra/storage_manager.hpp"

class TestUserService : public ::testing::Test
{
 protected:
    // You can define per-test set-up logic as needed.
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }

    // You can define per-test tear-down logic as needed.
    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(TestUserService, UserService_CreateAndFindUser)
{
    auto& service = silok::application::UserService::Get();

    service.Create("john.doe", "john.doe@test.com", "password");

    auto user = service.FindByEmail("john.doe@test.com");

    ASSERT_TRUE(user.has_value());
    ASSERT_EQ(user->name, "john.doe");
    ASSERT_EQ(user->email, "john.doe@test.com");
    ASSERT_NE(user->password, "password");

    ASSERT_EQ(service.Login("john.doe@test.com", "wrong_password"), std::nullopt);
    ASSERT_EQ(service.Login("wrong_email@test.com", "password"), std::nullopt);

    auto token = service.Login("john.doe@test.com", "password");
    ASSERT_TRUE(token.has_value());
}
TEST_F(TestUserService, UserService_DuplicateEmailShouldFail)
{
    auto& service = silok::application::UserService::Get();

    service.Create("john.doe", "john.doe@test.com", "password");

    // 예외를 던지도록 구현되었다면:
    EXPECT_THROW(
        { service.Create("duplicate", "john.doe@test.com", "newpassword"); }, std::runtime_error);
}
TEST_F(TestUserService, UserService_UpdateUser)
{
    auto& service = silok::application::UserService::Get();

    service.Create("alice", "alice@test.com", "password");

    auto user = service.FindByEmail("alice@test.com");
    ASSERT_TRUE(user.has_value());

    user->name = "Alice Smith";
    service.Update(*user);

    auto updated_user = service.FindByEmail("alice@test.com");
    ASSERT_TRUE(updated_user.has_value());
    EXPECT_EQ(updated_user->name, "Alice Smith");
}
TEST_F(TestUserService, UserService_DeleteUser)
{
    auto& service = silok::application::UserService::Get();

    service.Create("bob", "bob@test.com", "password");

    auto user = service.FindByEmail("bob@test.com");
    ASSERT_TRUE(user.has_value());

    service.Delete(*user);

    auto deleted = service.FindByEmail("bob@test.com");
    EXPECT_FALSE(deleted.has_value());

    auto token = service.Login("bob@test.com", "password");
    EXPECT_FALSE(token.has_value());
}
TEST_F(TestUserService, UserService_PasswordUpdate)
{
    auto& service = silok::application::UserService::Get();

    service.Create("changepw", "pw@test.com", "original");

    auto user = service.FindByEmail("pw@test.com");
    ASSERT_TRUE(user.has_value());

    user->password = "new_password";  // 암호화는 Update 내부에서 처리된다고 가정
    service.Update(*user);

    EXPECT_FALSE(service.Login("pw@test.com", "original").has_value());
    EXPECT_TRUE(service.Login("pw@test.com", "new_password").has_value());
}
