#include <gtest/gtest.h>

#include "silok/domain/model.hpp"
#include "silok/infra/repository/note_repository.hpp"
#include "silok/infra/storage_manager.hpp"

class NoteRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }
};

TEST_F(NoteRepositoryTest, CRUD)
{
    silok::infra::repository::NoteRepository repo;
    silok::domain::Note note{0, "note", 0, 0};

    auto id = repo.Create(note);
    EXPECT_GT(id, 0);

    auto found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->content, "note");

    note.id = id;
    note.content = "updated";
    EXPECT_TRUE(repo.Update(note));

    found = repo.FindById(id);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->content, "updated");

    auto list = repo.FindByIds({id});
    EXPECT_EQ(list.size(), 1);

    repo.Delete(id);
    EXPECT_FALSE(repo.FindById(id).has_value());
}
