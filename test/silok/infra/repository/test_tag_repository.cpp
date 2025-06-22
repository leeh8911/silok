#include <gtest/gtest.h>

#include "silok/domain/model.hpp"
#include "silok/infra/repository/tag_repository.hpp"
#include "silok/infra/storage_manager.hpp"

class TagRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }
};

TEST_F(TagRepositoryTest, CRUD)
{
    silok::infra::repository::TagRepository repo;
    silok::domain::Tag tag{0, "tag"};

    auto id = repo.Create(tag);
    EXPECT_GT(id, 0);

    auto found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->name, "tag");

    tag.id = id;
    tag.name = "updated";
    EXPECT_TRUE(repo.Update(tag));

    found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->name, "updated");

    auto list = repo.FindByIds({id});
    EXPECT_EQ(list.size(), 1);

    repo.Delete(id);
    EXPECT_FALSE(repo.FindById(id).has_value());
}
