#include <gtest/gtest.h>

#include "silok/manager/account_manager.hpp"
#include "silok/manager/note_manager.hpp"
#include "silok/manager/storage_manager.hpp"
#include "silok/manager/tag_manager.hpp"
#include "silok/model.hpp"

class TestTagManager : public ::testing::Test
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

TEST_F(TestTagManager, FR_29_Create_tag)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Test Tag";

    EXPECT_NO_THROW(manager.CreateTag(user_info, tag_name));
}

TEST_F(TestTagManager, FR_30_Update_tag)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Test Tag";
    manager.CreateTag(user_info, tag_name);

    auto tags = manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);

    auto& tag = tags.front();
    tag.name = "Updated Test Tag";
    manager.UpdateTag(user_info, tag);

    auto updated_tags = manager.GetAllTags(user_info);
    EXPECT_EQ(updated_tags.size(), 1);

    auto& updated_tag = updated_tags.front();
    EXPECT_EQ(updated_tag.name, "Updated Test Tag");
}

TEST_F(TestTagManager, FR_31_Delete_tag)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Test Tag";
    manager.CreateTag(user_info, tag_name);

    auto tags = manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);

    auto& tag = tags.front();
    manager.DeleteTag(user_info, tag);

    auto updated_tags = manager.GetAllTags(user_info);
    EXPECT_EQ(updated_tags.size(), 0);
}

TEST_F(TestTagManager, FR_32_Get_all_notes_from_tag)
{
    silok::manager::TagManager tag_manager{};
    silok::manager::NoteManager note_manager{};

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

TEST_F(TestTagManager, FR_36_Unique_tag_name)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Unique Tag";
    manager.CreateTag(user_info, tag_name);

    // Attempt to create a tag with the same name
    manager.CreateTag(user_info, tag_name);
    auto tags = manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);  // Ensure only one tag with the unique name exists

    // Create a different tag
    std::string another_tag_name = "Another Unique Tag";
    EXPECT_NO_THROW(manager.CreateTag(user_info, another_tag_name));
}