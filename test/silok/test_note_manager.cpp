#include <gtest/gtest.h>

#include "silok/manager/account_manager.hpp"
#include "silok/manager/note_manager.hpp"
#include "silok/manager/storage_manager.hpp"

class TestNoteManager : public ::testing::Test
{
 public:
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

TEST_F(TestNoteManager, FR_2_1_Create_note)
{
    silok::manager::NoteManager manager{};

    std::string content = "This is a test note.";

    EXPECT_NO_THROW(manager.CreateNote(content, user_info));
}

TEST_F(TestNoteManager, FR_2_2_Update_note)
{
    silok::manager::NoteManager manager{};

    std::string content = "This is a test note.";
    manager.CreateNote(content, user_info);

    auto notes = manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);

    auto& note = notes.front();

    note.content = "This is an updated test note.";
    manager.UpdateNote(note, user_info);

    auto updated_notes = manager.GetAllNotes(user_info);
    EXPECT_EQ(updated_notes.size(), 1);

    auto& updated_note = updated_notes.front();
    EXPECT_EQ(updated_note.content, "This is an updated test note.");
    EXPECT_EQ(note.created_at, updated_note.created_at);
    EXPECT_NE(note.updated_at, updated_note.updated_at);
}

TEST_F(TestNoteManager, FR_2_3_Delete_note)
{
    silok::manager::NoteManager manager{};

    std::string content = "This is a test note.";
    manager.CreateNote(content, user_info);

    auto notes = manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);

    auto& note = notes.front();
    EXPECT_NO_THROW(manager.DeleteNote(note, user_info));

    auto deleted_notes = manager.GetAllNotes(user_info);
    EXPECT_TRUE(deleted_notes.empty());
}