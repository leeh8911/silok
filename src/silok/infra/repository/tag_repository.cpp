#include "silok/infra/repository/tag_repository.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::infra::repository
{

using silok::domain::Tag;
using silok::infra::StorageManager;

int64_t TagRepository::Create(const Tag& tag)
{
    return StorageManager::Insert(tag);
}

std::optional<Tag> TagRepository::FindById(int64_t id)
{
    auto tags = StorageManager::FindByField<Tag>(&Tag::id, id);
    if (tags.empty())
    {
        return std::nullopt;
    }
    return tags.front();
}

std::vector<Tag> TagRepository::FindByIds(const std::vector<int64_t>& ids)
{
    return StorageManager::FindByFieldIn<Tag>(&Tag::id, ids);
}

bool TagRepository::Update(const Tag& tag)
{
    try
    {
        StorageManager::Update(tag);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void TagRepository::Delete(int64_t id)
{
    Tag tag = StorageManager::FindByField<Tag>(&Tag::id, id).front();
    StorageManager::Remove(tag);
}

}  // namespace silok::infra::repository
