#include <gtest/gtest.h>

#include "silok/domain/model.hpp"
#include "silok/infra/manager/account_manager.hpp"
#include "silok/infra/manager/note_manager.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/infra/manager/tag_manager.hpp"
#include "silok/infra/password_hasher/noop_password_hasher.hpp"

class TestTagManager : public ::testing::Test
{
 protected:
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

TEST_F(TestTagManager, FR_29_Create_tag)
{
    silok::infra::TagManager manager{};

    std::string tag_name = "Test Tag";

    EXPECT_NO_THROW(manager.CreateTag(user_info, tag_name));
}

TEST_F(TestTagManager, FR_30_Update_tag)
{
    silok::infra::TagManager manager{};

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
    silok::infra::TagManager manager{};

    std::string tag_name = "Test Tag";
    manager.CreateTag(user_info, tag_name);

    auto tags = manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);

    auto& tag = tags.front();

    EXPECT_NO_THROW(manager.DeleteTag(user_info, tag));

    auto updated_tags = manager.GetAllTags(user_info);
    EXPECT_EQ(updated_tags.size(), 0);
}

TEST_F(TestTagManager, FR_36_Unique_tag_name)
{
    silok::infra::TagManager manager{};

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
