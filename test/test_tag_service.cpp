#include <gtest/gtest.h>

#include "silok/application/note_service.hpp"
#include "silok/application/tag_service.hpp"
#include "silok/application/user_service.hpp"
#include "silok/domain/data.hpp"
#include "silok/domain/data_relation.hpp"

class TestTagService : public ::testing::Test
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

TEST_F(TestTagService, TagService_CreateAndFindAll)
{
    auto& user_service = silok::application::UserService::Get();

    user_service.Create("john.doe", "john.doe@test.com", "password");

    auto token = user_service.Login("john.doe@test.com", "password");
    ASSERT_TRUE(token.has_value());

    auto& tag_service = silok::application::TagService::Get();

    tag_service.Create("first_tag", token.value());

    auto tags = tag_service.FindAll(token.value());

    EXPECT_EQ(tags.size(), 1);
    EXPECT_EQ(tags[0].name, "first_tag");
}
