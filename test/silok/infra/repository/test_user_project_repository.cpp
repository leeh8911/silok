#include <gtest/gtest.h>

#include "silok/domain/model_relation.hpp"
#include "silok/infra/repository/user_project_repository.hpp"
#include "silok/infra/storage_manager.hpp"

class UserProjectRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }
};

TEST_F(UserProjectRepositoryTest, CRUD)
{
    silok::infra::repository::UserProjectRepository repo;
    silok::domain::UserProject rel{0, 1, 2};

    auto id = repo.Create(rel);
    EXPECT_GT(id, 0);

    auto found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->first_id, 1);

    rel.id = id;
    rel.second_id = 3;
    EXPECT_TRUE(repo.Update(rel));

    found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->second_id, 3);

    auto byFirst = repo.FindByFirstId(1);
    EXPECT_EQ(byFirst.size(), 1);

    auto bySecond = repo.FindBySecondId(3);
    EXPECT_EQ(bySecond.size(), 1);

    repo.Delete(id);
    EXPECT_FALSE(repo.FindById(id).has_value());
}
