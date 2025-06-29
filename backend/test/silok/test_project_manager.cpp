#include <gtest/gtest.h>

#include "silok/infra/manager/account_manager.hpp"
#include "silok/infra/manager/note_manager.hpp"
#include "silok/infra/manager/project_manager.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/infra/password_hasher/noop_password_hasher.hpp"

class TestProjectManager : public ::testing::Test
{
 protected:
    void SetUp() override
    {
        // Initialize the storage manager with a test database path
        silok::infra::StorageManager::Initialize(":memory:", true);

        account_manager.CreateAccount("john doe", "password123", "john.doe@test.com");

        auto user_token = account_manager.Login("john.doe@test.com", "password123");
        user_info = account_manager.GetAccountInfo(user_token.value()).value();
    }
    void TearDown() override
    {
        // Reset the storage manager after each test
        silok::infra::StorageManager::reset();
    }

    silok::infra::AccountManager account_manager{
        std::make_shared<silok::infra::NoOpPasswordHasher>()};
    silok::User user_info{};
};

TEST_F(TestProjectManager, FR_37_Create_project)
{
    silok::infra::ProjectManager manager{};

    std::string project_name = "Test Project";

    EXPECT_NO_THROW(manager.CreateProject(user_info, project_name));
}

TEST_F(TestProjectManager, FR_38_Update_project)
{
    silok::infra::ProjectManager manager{};

    std::string project_name = "Test Project";
    manager.CreateProject(user_info, project_name);

    auto projects = manager.GetAllProjects(user_info);
    EXPECT_EQ(projects.size(), 1);

    auto project = projects.front();
    project.name = "Updated Project Name";

    EXPECT_NO_THROW(manager.UpdateProject(user_info, project));

    auto updated_projects = manager.GetAllProjects(user_info);
    EXPECT_EQ(updated_projects.size(), 1);

    auto updated_project = updated_projects.front();
    EXPECT_NE(project_name, updated_project.name);
    EXPECT_EQ(updated_project.id, project.id);
}

TEST_F(TestProjectManager, FR_39_Delete_project)
{
    silok::infra::ProjectManager manager{};

    std::string project_name = "Test Project";
    manager.CreateProject(user_info, project_name);

    auto projects = manager.GetAllProjects(user_info);
    EXPECT_EQ(projects.size(), 1);

    auto project = projects.front();

    EXPECT_NO_THROW(manager.DeleteProject(user_info, project));

    auto remaining_projects = manager.GetAllProjects(user_info);
    EXPECT_EQ(remaining_projects.size(), 0);
}