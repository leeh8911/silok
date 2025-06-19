#include <gtest/gtest.h>

#include "silok/application/note_service.hpp"
#include "silok/application/user_service.hpp"
#include "silok/domain/user_token.hpp"
#include "silok/infra/storage_manager.hpp"

class TestNoteService : public ::testing::Test
{
 protected:
    // You can define per-test set-up logic as needed.
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);
    }

    // You can define per-test tear-down logic as needed.
    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(TestNoteService, NoteService_CreateAndFindAll)
{
    auto& user_service = silok::application::UserService::Get();

    user_service.Create("john.doe", "john.doe@test.com", "password");

    auto token = user_service.Login("john.doe@test.com", "password");
    ASSERT_TRUE(token.has_value());

    auto& note_service = silok::application::NoteService::Get();

    note_service.Create("First Note", token.value());

    auto notes = note_service.FindAll(token.value());

    EXPECT_EQ(notes.size(), 1);
    EXPECT_EQ(notes[0].content, "First Note");
}
