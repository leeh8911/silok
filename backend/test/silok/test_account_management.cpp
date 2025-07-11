#include <gtest/gtest.h>

#include <string>

#include "silok/infra/manager/account_manager.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/infra/password_hasher/noop_password_hasher.hpp"

class TestAccountManagement : public ::testing::Test
{
 public:
    void SetUp() override
    {
        // Initialize the storage manager with a test database path
        silok::infra::StorageManager::Initialize(":memory:", true);
        password_hasher = std::make_shared<silok::infra::NoOpPasswordHasher>();
    }
    void TearDown() override
    {
        // Reset the storage manager after each test
        silok::infra::StorageManager::reset();
    }
    silok::domain::PasswordHasherPtr password_hasher{};
};

TEST_F(TestAccountManagement, FR_4_Create_user_account)
{
    silok::infra::AccountManager manager{password_hasher};

    std::string username = "john doe";
    std::string password = "password123";
    std::string email = "john.doe@test.com";

    EXPECT_NO_THROW(manager.CreateAccount(username, password, email));
}

TEST_F(TestAccountManagement, FR_5_Login_user_account)
{
    silok::infra::AccountManager manager{password_hasher};

    std::string username = "john doe";
    std::string password = "password123";
    std::string email = "john.doe@test.com";

    manager.CreateAccount(username, password, email);

    auto token = manager.Login(email, password);
    EXPECT_TRUE(token.has_value());
}
TEST_F(TestAccountManagement, FR_6_Is_unique_user_email_information)
{
    silok::infra::AccountManager manager{password_hasher};
    {
        std::string username = "john doe";
        std::string password = "password123";
        std::string email = "john.doe@test.com";

        manager.CreateAccount(username, password, email);
    }
    {
        std::string username = "jane doe";
        std::string password = "somepassword";
        std::string email = "john.doe@test.com";  // Duplicate email

        EXPECT_THROW(manager.CreateAccount(username, password, email), std::runtime_error);
    }
}

TEST_F(TestAccountManagement, FR_7_Crypt_user_password)
{
    silok::infra::AccountManager manager{password_hasher};

    std::string username = "john doe";
    std::string password = "password123";
    std::string email = "john.doe@test.com";

    manager.CreateAccount(username, password, email);

    auto token = manager.Login(email, password);

    auto account_info = manager.GetAccountInfo(token.value());
    EXPECT_TRUE(account_info.has_value());

    EXPECT_EQ(account_info->name, username);
    EXPECT_EQ(account_info->email, email);
    EXPECT_NE(account_info->password, password);  // Password should be hashed
}

TEST_F(TestAccountManagement, FR_19_Delete_user_account)
{
    silok::infra::AccountManager manager{password_hasher};

    std::string username = "john doe";
    std::string password = "password123";
    std::string email = "john.doe@test.com";

    manager.CreateAccount(username, password, email);

    auto token = manager.Login(email, password);

    auto account_info = manager.GetAccountInfo(token.value());

    manager.DeleteAccount(account_info.value(), token.value());

    account_info = manager.GetAccountInfo(token.value());

    EXPECT_FALSE(account_info.has_value())
        << account_info.value().name;  // Account should be deleted
}
