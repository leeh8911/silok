#include <gtest/gtest.h>

#include "silok/application/user_service.hpp"
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
