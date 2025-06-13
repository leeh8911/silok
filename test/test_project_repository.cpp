#include <gtest/gtest.h>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/db/sql/sqlite_db_statement.hpp"
#include "silok/db/sql/sqlite_schema_manager.hpp"
#include "silok/repository/project_repository.hpp"
#include "silok/repository/user_repository.hpp"

class ProjectRepositoryTest : public ::testing::Test
{
 protected:
    silok::db::SqliteDBConnectionPtr db{nullptr};
    std::shared_ptr<silok::repository::ProjectRepository> project_repo{nullptr};
    std::shared_ptr<silok::repository::UserRepository> user_repo{nullptr};

    void SetUp() override
    {
        // Setup code for the test
        db = std::make_shared<silok::db::SqliteDBConnection>(":memory:");

        silok::db::SqliteSchemaManager schemaManager;
        schemaManager.migrate(*db);

        project_repo = std::make_shared<silok::repository::ProjectRepository>(db);
        user_repo = std::make_shared<silok::repository::UserRepository>(db);
    }

    void TearDown() override
    {
        // Cleanup code if needed
    }
};

TEST_F(ProjectRepositoryTest, BasicTagCRUDScenario)
{
    user_repo->createUser("testuser");
    auto user = user_repo->getUserByName("testuser").value();

    project_repo->createProject("testproject", user.id);

    auto projects = project_repo->getProjectsByOwner(user.id);
    EXPECT_EQ(projects.size(), 1);

    auto project = projects[0];
    EXPECT_GT(project.id, 0);
    EXPECT_EQ(project.name, "testproject");

    auto found = project_repo->getProjectById(project.id, user.id).value();
    EXPECT_EQ(found.id, project.id);
    EXPECT_EQ(found.name, "testproject");

    project_repo->deleteProject(project.id, user.id);
    {
        auto deleted = project_repo->getProjectById(project.id, user.id);
        EXPECT_FALSE(deleted.has_value());

        auto projects = project_repo->getProjectsByOwner(user.id);
        EXPECT_TRUE(projects.empty());  // Should be empty after deletion
    }
}

TEST_F(ProjectRepositoryTest, WrongOwnershipScenario)
{
    user_repo->createUser("owner");
    user_repo->createUser("shared_user");
    auto owner = user_repo->getUserByName("owner").value();
    auto shared_user = user_repo->getUserByName("shared_user").value();

    project_repo->createProject("shared_project", owner.id);
    auto projects = project_repo->getProjectsByOwner(owner.id);
    auto project = projects[0];

    auto owner_found = project_repo->getProjectById(project.id, owner.id).value();
    EXPECT_EQ(owner_found.id, project.id);
    EXPECT_EQ(owner_found.name, "shared_project");

    EXPECT_FALSE(project_repo->getProjectById(project.id, shared_user.id)
                     .has_value());  // Should not be found since shared_user has no access

    project_repo->shareProject(project.id, owner.id, shared_user.id);
    auto shared_found = project_repo->getProjectById(project.id, shared_user.id).value();
    EXPECT_EQ(shared_found.id, project.id);

    try
    {
        project_repo->deleteProject(project.id, shared_user.id);
        FAIL() << "Expected an exception when shared user tries to delete project";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_STREQ(e.what(), "User does not have permission to delete this project");
    }
}
