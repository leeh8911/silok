#include "silok/manager/project_manager.hpp"

#include <algorithm>
#include <vector>

#include "silok/manager/storage_manager.hpp"
#include "silok/manager/utils.hpp"

namespace silok::manager
{
void ProjectManager::CreateProject(const User user, const std::string& project_name,
                                   std::optional<int64_t> start, std::optional<int64_t> end)
{
    Project project;
    project.name = project_name;
    project.start = start;
    project.end = end;

    auto project_id = StorageManager::Insert(project);

    UserProject user_project;
    user_project.user_id = user.id;
    user_project.project_id = project_id;
    user_project.role = "owner";  // Default role for the creator

    StorageManager::Insert(user_project);
}

std::vector<Project> ProjectManager::GetAllProjects(const User user) const
{
    auto user_projects = StorageManager::FindByField<UserProject>(&UserProject::user_id, user.id);

    std::vector<int64_t> project_ids;
    std::transform(user_projects.begin(), user_projects.end(), std::back_inserter(project_ids),
                   [](const UserProject& up) { return up.project_id; });

    return StorageManager::FindByFieldIn<Project>(&Project::id, project_ids);
}

void ProjectManager::UpdateProject(const User user, Project project)
{
    if (!utils::HasPermission<UserProject>(user, project, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to update project ID: {}", user.email,
                        project.id);
        return;
    }

    SILOK_LOG_INFO("User {} is updating project '{}...'", user.email, project.name.substr(0, 20));
    StorageManager::Update(project);
}

void ProjectManager::DeleteProject(const User user, Project project)
{
    if (!utils::HasPermission<UserProject>(user, project, "owner"))
    {
        SILOK_LOG_ERROR("User {} does not have permission to delete project ID: {}", user.email,
                        project.id);
        return;
    }

    SILOK_LOG_INFO("User {} is deleting project '{}...'", user.email, project.name.substr(0, 20));
    StorageManager::Remove(project);
}
}  // namespace silok::manager
