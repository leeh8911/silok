#include <gtest/gtest.h>

#include "silok/infra/manager/account_manager.hpp"
#include "silok/infra/manager/note_manager.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/infra/manager/tag_manager.hpp"
#include "silok/infra/password_hasher/noop_password_hasher.hpp"

class TestNoteManager : public ::testing::Test
{
 public:
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

TEST_F(TestNoteManager, FR_18_Create_note)
{
    silok::infra::NoteManager manager{};

    std::string content = "This is a test note.";

    EXPECT_NO_THROW(manager.CreateNote(user_info, content));
}

TEST_F(TestNoteManager, FR_22_Update_note)
{
    silok::infra::NoteManager manager{};

    std::string content = "This is a test note.";
    manager.CreateNote(user_info, content);

    auto notes = manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);

    auto& note = notes.front();

    note.content = "This is an updated test note.";
    manager.UpdateNote(user_info, note);

    auto updated_notes = manager.GetAllNotes(user_info);
    EXPECT_EQ(updated_notes.size(), 1);

    auto& updated_note = updated_notes.front();
    EXPECT_EQ(updated_note.content, "This is an updated test note.");
    EXPECT_EQ(note.created_at, updated_note.created_at);
    EXPECT_NE(note.updated_at, updated_note.updated_at);
}

TEST_F(TestNoteManager, FR_23_Delete_note)
{
    silok::infra::NoteManager manager{};

    std::string content = "This is a test note.";
    manager.CreateNote(user_info, content);

    auto notes = manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);

    auto& note = notes.front();
    EXPECT_NO_THROW(manager.DeleteNote(user_info, note));

    auto deleted_notes = manager.GetAllNotes(user_info);
    EXPECT_TRUE(deleted_notes.empty());
}

TEST_F(TestNoteManager, FR_24_Link_note_with_tag)
{
    silok::infra::NoteManager note_manager{};
    silok::infra::TagManager tag_manager{};

    std::string content = "This is a test note.";
    note_manager.CreateNote(user_info, content);

    std::string tag_name = "Test Tag";
    tag_manager.CreateTag(user_info, tag_name);

    auto notes = note_manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);
    auto& note = notes.front();

    auto tags = tag_manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);
    auto& tag = tags.front();

    EXPECT_NO_THROW(note_manager.LinkNoteToTag(user_info, note, tag));

    auto linked_notes = note_manager.GetAllNotesByTag(user_info, tag);
    EXPECT_EQ(linked_notes.size(), 1);

    auto& linked_note = linked_notes.front();
    EXPECT_EQ(linked_note.id, note.id);
}

TEST_F(TestNoteManager, FR_25_Unlink_note_from_tag)
{
    silok::infra::NoteManager note_manager{};
    silok::infra::TagManager tag_manager{};

    std::string content = "This is a test note.";
    note_manager.CreateNote(user_info, content);

    std::string tag_name = "Test Tag";
    tag_manager.CreateTag(user_info, tag_name);

    auto notes = note_manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);
    auto& note = notes.front();

    auto tags = tag_manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);
    auto& tag = tags.front();

    note_manager.LinkNoteToTag(user_info, note, tag);

    auto linked_notes = note_manager.GetAllNotesByTag(user_info, tag);
    EXPECT_EQ(linked_notes.size(), 1);

    EXPECT_NO_THROW(note_manager.UnlinkNoteFromTag(user_info, note, tag));

    linked_notes = note_manager.GetAllNotesByTag(user_info, tag);
    EXPECT_TRUE(linked_notes.empty());
}

TEST_F(TestNoteManager, FR_34_Note_tag_relation_reliability)
{
    silok::infra::NoteManager note_manager{};
    silok::infra::TagManager tag_manager{};

    std::string content = "This is a test note.";
    note_manager.CreateNote(user_info, content);

    std::string tag_name = "Test Tag";
    tag_manager.CreateTag(user_info, tag_name);

    auto notes = note_manager.GetAllNotes(user_info);
    EXPECT_EQ(notes.size(), 1);
    auto& note = notes.front();

    auto tags = tag_manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);
    auto& tag = tags.front();

    note_manager.LinkNoteToTag(user_info, note, tag);

    auto linked_notes = note_manager.GetAllNotesByTag(user_info, tag);
    EXPECT_EQ(linked_notes.size(), 1);

    EXPECT_NO_THROW(note_manager.UnlinkNoteFromTag(user_info, note, tag));

    linked_notes = note_manager.GetAllNotesByTag(user_info, tag);
    EXPECT_TRUE(linked_notes.empty());
}

TEST_F(TestNoteManager, FR_32_Get_all_notes_from_tag)
{
    silok::infra::TagManager tag_manager{};
    silok::infra::NoteManager note_manager{};

    note_manager.CreateNote(user_info, "Test note 1");
    note_manager.CreateNote(user_info, "Test note 2");
    note_manager.CreateNote(user_info, "Test note 3");
    tag_manager.CreateTag(user_info, "test_tag");

    auto notes = note_manager.GetAllNotes(user_info);

    auto tags = tag_manager.GetAllTags(user_info);
    auto& tag = tags.front();

    for (auto it = notes.begin(); it != notes.begin() + 2; ++it)
    {
        note_manager.LinkNoteToTag(user_info, *it, tag);
    }

    auto tagged_notes = note_manager.GetAllNotesByTag(user_info, tag);
    EXPECT_EQ(tagged_notes.size(), 2);

    for (const auto& note : tagged_notes)
    {
        EXPECT_TRUE(std::any_of(notes.begin(), notes.end(),
                                [&note](const silok::Note& n) { return n.id == note.id; }));
    }
}
