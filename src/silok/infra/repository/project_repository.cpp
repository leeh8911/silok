#include "silok/infra/repository/project_repository.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::infra::repository
{

using silok::domain::Project;
using silok::infra::StorageManager;

int64_t ProjectRepository::Create(const Project& project)
{
    return StorageManager::Insert(project);
}

std::optional<Project> ProjectRepository::FindById(int64_t id)
{
    auto projects = StorageManager::FindByField<Project>(&Project::id, id);
    if (projects.empty())
    {
        return std::nullopt;
    }
    return projects.front();
}

std::vector<Project> ProjectRepository::FindByIds(const std::vector<int64_t>& ids)
{
    return StorageManager::FindByFieldIn<Project>(&Project::id, ids);
}

bool ProjectRepository::Update(const Project& project)
{
    try
    {
        StorageManager::Update(project);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void ProjectRepository::Delete(int64_t id)
{
    Project project = StorageManager::FindByField<Project>(&Project::id, id).front();
    StorageManager::Remove(project);
}

}  // namespace silok::infra::repository
