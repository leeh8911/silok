#include <gtest/gtest.h>

#include "silok/domain/model.hpp"
#include "silok/infra/repository/project_repository.hpp"
#include "silok/infra/storage_manager.hpp"

class ProjectRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }
};

TEST_F(ProjectRepositoryTest, CRUD)
{
    silok::infra::repository::ProjectRepository repo;
    silok::domain::Project project{0, "proj", 1, 2};

    auto id = repo.Create(project);
    EXPECT_GT(id, 0);

    auto found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->name, "proj");

    project.id = id;
    project.name = "updated";
    EXPECT_TRUE(repo.Update(project));

    found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->name, "updated");

    auto list = repo.FindByIds({id});
    EXPECT_EQ(list.size(), 1);

    repo.Delete(id);
    EXPECT_FALSE(repo.FindById(id).has_value());
}
