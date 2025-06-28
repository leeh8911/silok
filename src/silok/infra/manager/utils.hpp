#pragma once

#include <cstdint>
#include <string>

#include "silok/domain/model.hpp"

namespace silok::infra::utils
{
template <typename T>
struct RelationTraits;

template <>
struct RelationTraits<UserNote>
{
    static int64_t UserNote::* user_id()
    {
        return &UserNote::user_id;
    }
    static int64_t UserNote::* resource_id()
    {
        return &UserNote::note_id;
    }
    static std::string UserNote::* role()
    {
        return &UserNote::role;
    }
};

template <>
struct RelationTraits<UserTag>
{
    static int64_t UserTag::* user_id()
    {
        return &UserTag::user_id;
    }
    static int64_t UserTag::* resource_id()
    {
        return &UserTag::tag_id;
    }
    static std::string UserTag::* role()
    {
        return &UserTag::role;
    }
};

template <>
struct RelationTraits<UserProject>
{
    static int64_t UserProject::* user_id()
    {
        return &UserProject::user_id;
    }
    static int64_t UserProject::* resource_id()
    {
        return &UserProject::project_id;
    }
    static std::string UserProject::* role()
    {
        return &UserProject::role;
    }
};

template <typename Relation, typename UserT, typename ResourceT>
bool HasPermission(const UserT& user, const ResourceT& resource,
                   const std::string& required_role = "owner")
{
    auto relations = StorageManager::FindByFields<Relation>(
        RelationTraits<Relation>::user_id(), user.id, RelationTraits<Relation>::resource_id(),
        resource.id);

    if (relations.empty())
    {
        SILOK_LOG_ERROR("User does not have any relation to entity ID: {}", resource.id);
        return false;
    }

    if (relations.front().*RelationTraits<Relation>::role() != required_role)
    {
        SILOK_LOG_ERROR("User does not have '{}' role for entity ID: {}", required_role,
                        resource.id);
        return false;
    }

    return true;
}

}  // namespace silok::infra::utils
