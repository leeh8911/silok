#include "tag_manager.hpp"

#include <algorithm>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/infra/manager/storage_manager.hpp"
#include "silok/infra/manager/utils.hpp"

namespace silok::infra
{

void TagManager::CreateTag(const User user, const std::string& tag_name)
{
    Tag tag;
    tag.name = tag_name;

    auto user_tags =
        StorageManager::FindByFields<UserTag>(&UserTag::user_id, user.id, &UserTag::role, "owner");
    if (!user_tags.empty())
    {
        std::vector<int64_t> tag_ids{};
        std::transform(user_tags.begin(), user_tags.end(), std::back_inserter(tag_ids),
                       [](const UserTag& ut) { return ut.tag_id; });

        auto tags = StorageManager::FindByFieldIn<Tag>(&Tag::id, tag_ids);
        if (std::any_of(tags.begin(), tags.end(),
                        [&tag_name](const Tag& tag) { return tag.name == tag_name; }))
        {
            SILOK_LOG_ERROR("Tag '{}' already exists for user ID: {}", tag_name, user.id);
            return;
        }
    }

    auto tag_id = StorageManager::Insert(tag);

    UserTag user_tag;
    user_tag.user_id = user.id;
    user_tag.tag_id = tag_id;
    user_tag.role = "owner";  // Default role for the creator

    StorageManager::Insert(user_tag);
}

std::vector<Tag> TagManager::GetAllTags(const User user) const
{
    auto user_tags = StorageManager::FindByField<UserTag>(&UserTag::user_id, user.id);
    std::vector<int64_t> tag_ids{};
    std::transform(user_tags.begin(), user_tags.end(), std::back_inserter(tag_ids),
                   [](const UserTag& ut) { return ut.tag_id; });

    std::vector<Tag> tags = StorageManager::FindByFieldIn<Tag>(&Tag::id, tag_ids);
    return tags;
}

void TagManager::UpdateTag(const User user, Tag tag)
{
    if (!utils::HasPermission<UserTag>(user, tag, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to update tag ID: {}", user.email,
                        tag.id);
        return;
    }

    SILOK_LOG_INFO("User {} is updating tag '{}'", user.email, tag.name);
    StorageManager::Update(tag);
}

void TagManager::DeleteTag(const User user, Tag tag)
{
    if (!utils::HasPermission<UserTag>(user, tag, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to delete tag ID: {}", user.email,
                        tag.id);
        return;
    }

    SILOK_LOG_INFO("User {} is deleting tag '{}'", user.email, tag.name);
    StorageManager::Remove(tag);
}

}  // namespace silok::infra
