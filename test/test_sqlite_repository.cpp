#include <gtest/gtest.h>

#include "silok/repository/sqlite_repository.hpp"

class SqliteRepositoryTest : public ::testing::Test
{
 protected:
    silok::repository::SqliteRepository repo{":memory:"};

    void SetUp() override
    {
        // Code to set up the test environment, if needed
    }

    void TearDown() override
    {
        // Code to clean up after tests, if needed
    }
};

TEST_F(SqliteRepositoryTest, CreateNote)
{
    uint64_t note_id = repo.createNote();
    std::string content = "Test note content";

    auto note = repo.getNote(note_id);
    EXPECT_EQ(note.id, note_id);

    note.content = content;
    repo.updateNote(note);

    auto updated_note = repo.getNote(note_id);

    EXPECT_EQ(updated_note.content, content);
    EXPECT_EQ(updated_note.id, note.id);
    EXPECT_EQ(updated_note.created_at, note.created_at);
    EXPECT_NE(updated_note.updated_at, note.updated_at);
}
