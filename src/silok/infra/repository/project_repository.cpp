#include "silok/infra/repository/project_repository.hpp"

namespace silok::infra::repository
{

int64_t ProjectRepository::Create(const silok::domain::Project& project)
{
    return infra::StorageManager::Insert(project);
}

std::optional<silok::domain::Project> ProjectRepository::FindById(int64_t id)
{
    auto projects =
        infra::StorageManager::FindByField<silok::domain::Project>(&silok::domain::Project::id, id);
    if (projects.empty())
    {
        return std::nullopt;
    }
    return projects.front();
}

std::vector<silok::domain::Project> ProjectRepository::FindByIds(const std::vector<int64_t>& ids)
{
    return infra::StorageManager::FindByFieldIn<silok::domain::Project>(&silok::domain::Project::id,
                                                                        ids);
}

void ProjectRepository::Update(const silok::domain::Project& project)
{
    infra::StorageManager::Update(project);
}

void ProjectRepository::Delete(int64_t id)
{
    silok::domain::Project project =
        infra::StorageManager::FindByField<silok::domain::Project>(&silok::domain::Project::id, id)
            .front();
    infra::StorageManager::Remove(project);
}

}  // namespace silok::infra::repository
