#include "silok/repository/project_repository.hpp"

#include "silok/db/base/base_db_connection.hpp"
#include "silok/db/base/base_db_statement.hpp"
#include "silok/logger.hpp"

namespace silok::repository
{
ProjectRepository::ProjectRepository(silok::db::BaseDBConnectionPtr db_) : db(std::move(db_)) {}

void ProjectRepository::createProject(const std::string& name, uint64_t owner_id)
{
    auto stmt = db->prepare("INSERT INTO projects(name) VALUES(?);");
    stmt->bind(1, name);
    if (!stmt->step())
    {
        throw std::runtime_error("Failed to create project: " + name);
    }

    // Get the last inserted ID
    uint64_t project_id = db->lastInsertId();

    // Associate the project with the owner
    shareProject(project_id, owner_id, owner_id);
}

std::optional<std::vector<Project>> ProjectRepository::getProjectsByOwner(uint64_t owner_id)
{
    auto stmt = db->prepare(
        "SELECT id, name FROM projects WHERE id IN "
        "(SELECT project_id FROM user_project WHERE user_id = ?);");
    stmt->bind(1, owner_id);

    std::vector<Project> projects;
    while (stmt->step())
    {
        Project project;
        project.id = stmt->columnInt(0);
        project.name = stmt->columnText(1);
        projects.push_back(project);
    }

    if (projects.empty())
    {
        return std::nullopt;  // No projects found for the owner
    }
    return projects;
}

std::optional<Project> ProjectRepository::getProjectById(uint64_t id, uint64_t owner_id)
{
    auto stmt = db->prepare("SELECT id, name FROM projects WHERE id = ?;");
    stmt->bind(1, id);
    if (stmt->step())
    {
        Project project;
        project.id = stmt->columnInt(0);
        project.name = stmt->columnText(1);
        return project;
    }
    return std::nullopt;  // Project not found
}

void ProjectRepository::deleteProject(uint64_t id, uint64_t owner_id)
{
    // Check if the project exists and belongs to the owner
    auto project = getProjectById(id, owner_id);
    if (!project.has_value())
    {
        throw std::runtime_error("Project not found or does not belong to the user");
    }

    // Delete the project
    auto stmt = db->prepare("DELETE FROM projects WHERE id = ?;");
    stmt->bind(1, id);
    if (!stmt->step())
    {
        throw std::runtime_error("Failed to delete project with ID: " + std::to_string(id));
    }
}

void ProjectRepository::shareProject(uint64_t project_id, uint64_t owner_id,
                                     uint64_t shared_user_id)
{
    // Check if the project exists and belongs to the owner
    auto project = getProjectById(project_id, owner_id);
    if (!project.has_value())
    {
        throw std::runtime_error("Project not found or does not belong to the user");
    }

    // Share the project with the user
    auto stmt = db->prepare("INSERT INTO project_shares(project_id, user_id) VALUES(?, ?);");
    stmt->bind(1, project_id);
    stmt->bind(2, shared_user_id);
    if (!stmt->step())
    {
        throw std::runtime_error("Failed to share project with user ID: " +
                                 std::to_string(shared_user_id));
    }
}
}  // namespace silok::repository
