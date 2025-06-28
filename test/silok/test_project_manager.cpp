#include <gtest/gtest.h>

#include "silok/manager/account_manager.hpp"
#include "silok/manager/note_manager.hpp"
#include "silok/manager/project_manager.hpp"
#include "silok/manager/storage_manager.hpp"

class TestProjectManager : public ::testing::Test
{
 protected:
    void SetUp() override
    {
        // Initialize the storage manager with a test database path
        silok::manager::StorageManager::Initialize(":memory:", true);

        account_manager.CreateAccount("john doe", "password123", "john.doe@test.com");

        user_info =
            account_manager
                .GetAccountInfo(account_manager.Login("john.doe@test.com", "password123").value())
                .value();
    }
    void TearDown() override
    {
        // Reset the storage manager after each test
        silok::manager::StorageManager::reset();
    }

    silok::manager::AccountManager account_manager{};
    silok::User user_info{};
};

TEST_F(TestProjectManager, FR_37_Create_project)
{
    silok::manager::ProjectManager manager{};

    std::string project_name = "Test Project";

    EXPECT_NO_THROW(manager.CreateProject(user_info, project_name));
}