#include "silok/manager/tag_manager.hpp"

#include <algorithm>
#include <vector>

#include "silok/manager/storage_manager.hpp"
#include "silok/manager/utils.hpp"
#include "silok/model.hpp"

namespace silok::manager
{
void TagManager::CreateTag(const std::string& tag_name, const User user)
{
    Tag tag;
    tag.name = tag_name;

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

void TagManager::UpdateTag(Tag tag, const User user)
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

void TagManager::DeleteTag(Tag tag, const User user)
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

}  // namespace silok::manager
