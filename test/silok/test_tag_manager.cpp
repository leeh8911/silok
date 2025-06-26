#include <gtest/gtest.h>

#include "silok/manager/account_manager.hpp"
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

TEST_F(TestTagManager, FR_3_1_Create_tag)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Test Tag";

    EXPECT_NO_THROW(manager.CreateTag(tag_name, user_info));
}

TEST_F(TestTagManager, FR_3_2_Update_tag)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Test Tag";
    manager.CreateTag(tag_name, user_info);

    auto tags = manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);

    auto& tag = tags.front();
    tag.name = "Updated Test Tag";
    manager.UpdateTag(tag, user_info);

    auto updated_tags = manager.GetAllTags(user_info);
    EXPECT_EQ(updated_tags.size(), 1);

    auto& updated_tag = updated_tags.front();
    EXPECT_EQ(updated_tag.name, "Updated Test Tag");
}

TEST_F(TestTagManager, FR_3_3_Delete_tag)
{
    silok::manager::TagManager manager{};

    std::string tag_name = "Test Tag";
    manager.CreateTag(tag_name, user_info);

    auto tags = manager.GetAllTags(user_info);
    EXPECT_EQ(tags.size(), 1);

    auto& tag = tags.front();
    manager.DeleteTag(tag, user_info);

    auto updated_tags = manager.GetAllTags(user_info);
    EXPECT_EQ(updated_tags.size(), 0);
}