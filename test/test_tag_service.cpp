#include <gtest/gtest.h>

#include "../src/silok/domain/model.hpp"
#include "../src/silok/domain/model_relation.hpp"
#include "silok/application/note_service.hpp"
#include "silok/application/tag_service.hpp"
#include "silok/application/user_service.hpp"
#include "silok/infra/repository/user_repository.hpp"

class TestTagService : public ::testing::Test
{
 protected:
    // You can define per-test set-up logic as needed.
    void SetUp() override
    {
        silok::infra::StorageManager::Initialize(":memory:", true);

        user_service = silok::application::UserService(
            std::make_shared<silok::infra::repository::UserRepository>());
    }

    // You can define per-test tear-down logic as needed.
    void TearDown() override
    {
        // Code here will be called immediately after each test (right before the destructor).
    }

    silok::application::UserService user_service;
};

TEST_F(TestTagService, TagService_CreateAndFindAll)
{
    user_service.Create("john.doe", "john.doe@test.com", "password");

    auto token = user_service.Login("john.doe@test.com", "password");
    ASSERT_TRUE(token.has_value());

    auto& tag_service = silok::application::TagService::Get();

    tag_service.Create("first_tag", token.value());

    auto tags = tag_service.FindAll(token.value());

    EXPECT_EQ(tags.size(), 1);
    EXPECT_EQ(tags[0].name, "first_tag");
}
TEST_F(TestTagService, CreateTag_InvalidToken)
{
    auto& tag_service = silok::application::TagService::Get();
    EXPECT_THROW(tag_service.Create("InvalidTag", "bad_token", std::nullopt), std::runtime_error);
}

TEST_F(TestTagService, DeleteTag_NotOwned)
{
    user_service.Create("bob", "bob@example.com", "pass");
    user_service.Create("alice", "alice@example.com", "pass");

    auto bob_token = user_service.Login("bob@example.com", "pass");
    auto alice_token = user_service.Login("alice@example.com", "pass");

    auto& tag_service = silok::application::TagService::Get();
    tag_service.Create("BobTag", bob_token.value(), std::nullopt);

    auto tags = tag_service.FindAll(bob_token.value());
    ASSERT_EQ(tags.size(), 1);

    EXPECT_NO_THROW(tag_service.Delete(tags[0].id, alice_token.value()));
    // 현재는 user_tag 1개만 지우기 때문에 소유자가 아니어도 삭제 가능 → 위험 요소
}

TEST_F(TestTagService, DeleteTag_InvalidId)
{
    user_service.Create("charlie", "charlie@example.com", "pass");
    auto token = user_service.Login("charlie@example.com", "pass");

    auto& tag_service = silok::application::TagService::Get();
    EXPECT_THROW(tag_service.Delete(9999, token.value()), std::runtime_error);
}

TEST_F(TestTagService, CreateTag_WithoutNote)
{
    user_service.Create("dave", "dave@example.com", "pass");
    auto token = user_service.Login("dave@example.com", "pass");

    auto& tag_service = silok::application::TagService::Get();
    EXPECT_NO_THROW(tag_service.Create("Note-less Tag", token.value(), std::nullopt));
}

TEST_F(TestTagService, UpdateTag_NoteAssociation)
{
    user_service.Create("erin", "erin@example.com", "pass");
    auto token = user_service.Login("erin@example.com", "pass");

    auto& tag_service = silok::application::TagService::Get();
    tag_service.Create("OldName", token.value(), std::nullopt);
    auto tags = tag_service.FindAll(token.value());

    silok::domain::Tag tag = tags[0];
    tag.name = "UpdatedName";

    // 임의로 잘못된 노트와 연결 시도 (Note가 DB에 없음)
    silok::domain::Note fake_note;
    fake_note.id = 12345;

    EXPECT_NO_THROW(tag_service.Update(tag, fake_note));
    // 관계는 추가되나, 해당 note_id가 유효하지 않아 조회 시 혼란 가능
}

TEST_F(TestTagService, UpdateTag_InvalidTagId)
{
    auto& tag_service = silok::application::TagService::Get();

    silok::domain::Tag fake_tag;
    fake_tag.id = -1;
    fake_tag.name = "Should Fail";

    EXPECT_THROW(tag_service.Update(fake_tag, std::nullopt), std::runtime_error);
}

TEST_F(TestTagService, DeleteTag_ThenVerifyAbsence)
{
    user_service.Create("frank", "frank@example.com", "pass");
    auto token = user_service.Login("frank@example.com", "pass");

    auto& tag_service = silok::application::TagService::Get();
    tag_service.Create("Temporary", token.value(), std::nullopt);

    auto tags = tag_service.FindAll(token.value());
    ASSERT_EQ(tags.size(), 1);

    tag_service.Delete(tags[0].id, token.value());

    auto after = tag_service.FindAll(token.value());
    EXPECT_TRUE(after.empty());
}
