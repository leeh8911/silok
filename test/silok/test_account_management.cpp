#include <gtest/gtest.h>

#include <string>

#include "silok/account_manager.hpp"
#include "silok/storage_manager.hpp"

class TestAccountManagement : public ::testing::Test
{
 public:
    void SetUp() override
    {
        // Initialize the storage manager with a test database path
        silok::StorageManager::Initialize(":memory:", true);
    }
    void TearDown() override
    {
        // Reset the storage manager after each test
        silok::StorageManager::reset();
    }
};

TEST_F(TestAccountManagement, FR_1_1_Create_user_account)
{
    silok::AccountManager manager{};

    std::string username = "john doe";
    std::string password = "password123";
    std::string email = "john.doe@test.com";

    EXPECT_NO_THROW(manager.CreateAccount(username, password, email));
}

TEST_F(TestAccountManagement, FR_1_2_Login_user_account)
{
    silok::AccountManager manager{};

    std::string username = "john doe";
    std::string password = "password123";
    std::string email = "john.doe@test.com";

    manager.CreateAccount(username, password, email);

    auto token = manager.Login(email, password);
    EXPECT_TRUE(token.has_value());
}
