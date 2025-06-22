#include "silok/infra/repository/base_tag_repository.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::infra::repository
{
int64_t TagRepository::Create(const silok::domain::Tag& tag)
{
    return infra::StorageManager::Insert(tag);
}

std::optional<silok::domain::Tag> TagRepository::FindById(int64_t id)
{
    auto tags = infra::StorageManager::FindByField<silok::domain::Tag>(&silok::domain::Tag::id, id);
    if (tags.empty())
    {
        return std::nullopt;
    }
    return tags.front();
}

std::vector<silok::domain::Tag> TagRepository::FindByIds(const std::vector<int64_t>& ids)
{
    return infra::StorageManager::FindByFieldIn<silok::domain::Tag>(&silok::domain::Tag::id, ids);
}

void TagRepository::Update(const silok::domain::Tag& tag)
{
    infra::StorageManager::Update(tag);
}

void TagRepository::Delete(int64_t id)
{
    silok::domain::Tag tag =
        infra::StorageManager::FindByField<silok::domain::Tag>(&silok::domain::Tag::id, id).front();
    infra::StorageManager::Remove(tag);
}

}  // namespace silok::infra::repository
