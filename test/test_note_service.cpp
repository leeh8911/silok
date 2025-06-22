#include <gtest/gtest.h>

#include "silok/application/account_service.hpp"
#include "silok/application/note_service.hpp"
#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/infra/repository/user_repository.hpp"
#include "silok/infra/storage_manager.hpp"

class TestNoteService : public ::testing::Test
{
 protected:
    // You can define per-test set-up logic as needed.
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);

        this->user_service = silok::application::AccountService(
            std::make_shared<silok::infra::repository::UserRepository>());
    }

    // You can define per-test tear-down logic as needed.
    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }

    silok::application::AccountService user_service;
};

TEST_F(TestNoteService, NoteService_CreateAndFindAll)
{
    user_service.Create("john.doe", "john.doe@test.com", "password");

    auto token = user_service.Login("john.doe@test.com", "password");
    ASSERT_TRUE(token.has_value());

    auto& note_service = silok::application::NoteService::Get();

    note_service.Create("First Note", token.value());

    auto notes = note_service.FindAll(token.value());

    EXPECT_EQ(notes.size(), 1);
    EXPECT_EQ(notes[0].content, "First Note");
}
TEST_F(TestNoteService, NoteService_MultipleNotes)
{
    user_service.Create("alice", "alice@example.com", "pass");
    auto token = user_service.Login("alice@example.com", "pass");
    ASSERT_TRUE(token.has_value());

    auto& note_service = silok::application::NoteService::Get();

    note_service.Create("Note 1", token.value());
    note_service.Create("Note 2", token.value());
    note_service.Create("Note 3", token.value());

    auto notes = note_service.FindAll(token.value());
    EXPECT_EQ(notes.size(), 3);
}
TEST_F(TestNoteService, NoteService_OnlyOwnNotesReturned)
{
    user_service.Create("bob", "bob@example.com", "pass");
    user_service.Create("carol", "carol@example.com", "pass");

    auto bob_token = user_service.Login("bob@example.com", "pass");
    auto carol_token = user_service.Login("carol@example.com", "pass");

    auto& note_service = silok::application::NoteService::Get();

    note_service.Create("Bob's Note", bob_token.value());
    note_service.Create("Carol's Note", carol_token.value());

    auto bob_notes = note_service.FindAll(bob_token.value());
    auto carol_notes = note_service.FindAll(carol_token.value());

    EXPECT_EQ(bob_notes.size(), 1);
    EXPECT_EQ(carol_notes.size(), 1);
    EXPECT_EQ(bob_notes[0].content, "Bob's Note");
    EXPECT_EQ(carol_notes[0].content, "Carol's Note");
}
TEST_F(TestNoteService, NoteService_UpdateNote)
{
    user_service.Create("dave", "dave@example.com", "pass");
    auto token = user_service.Login("dave@example.com", "pass");

    auto& note_service = silok::application::NoteService::Get();
    note_service.Create("Original", token.value());

    auto notes = note_service.FindAll(token.value());
    ASSERT_EQ(notes.size(), 1);

    auto note = notes[0];
    note.content = "Updated Content";

    note_service.Update(note);

    auto updated_notes = note_service.FindAll(token.value());
    EXPECT_EQ(updated_notes.size(), 1);
    EXPECT_EQ(updated_notes[0].content, "Updated Content");
}
TEST_F(TestNoteService, NoteService_DeleteNote)
{
    user_service.Create("erin", "erin@example.com", "pass");
    auto token = user_service.Login("erin@example.com", "pass");

    auto& note_service = silok::application::NoteService::Get();
    note_service.Create("Note to be deleted", token.value());

    auto notes = note_service.FindAll(token.value());
    ASSERT_EQ(notes.size(), 1);

    note_service.Delete(notes[0].id, token.value());

    auto notes_after_delete = note_service.FindAll(token.value());
    EXPECT_TRUE(notes_after_delete.empty());
}
