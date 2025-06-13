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
    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to create project: " + name);
    }

    uint64_t project_id = db->lastInsertId();
    shareProject(project_id, owner_id, owner_id);
}

std::vector<Project> ProjectRepository::getProjectsByOwner(uint64_t owner_id)
{
    auto stmt = db->prepare(R"(
        SELECT p.id, p.name, up.role
        FROM projects p
        JOIN user_project up ON p.id = up.project_id
        WHERE up.user_id = ?
    )");
    stmt->bind(1, owner_id);

    std::vector<Project> projects{};
    while (stmt->step() == silok::db::StepResult::Row)
    {
        Project project;
        project.id = stmt->columnInt(0);
        project.name = stmt->columnText(1);
        project.role = Role::fromString(stmt->columnText(2));
        projects.push_back(project);
    }
    return projects;
}

std::optional<Project> ProjectRepository::getProjectById(uint64_t id, uint64_t user_id)
{
    auto stmt = db->prepare(R"(
        SELECT p.id, p.name, up.role
        FROM projects p
        JOIN user_project up ON p.id = up.project_id
        WHERE p.id = ? AND up.user_id = ?
    )");
    stmt->bind(1, id);
    stmt->bind(2, user_id);

    if (stmt->step() == silok::db::StepResult::Row)
    {
        Project project;
        project.id = stmt->columnInt(0);
        project.name = stmt->columnText(1);
        project.role = Role::fromString(stmt->columnText(2));
        return project;
    }

    return std::nullopt;
}

void ProjectRepository::deleteProject(uint64_t id, uint64_t owner_id)
{
    if (!isProjectOwned(id, owner_id))
    {
        throw std::runtime_error("User does not have permission to delete this project");
    }
    if (!getProjectById(id, owner_id).has_value())
    {
        throw std::runtime_error("Project not found or does not belong to the user");
    }

    auto stmt = db->prepare("DELETE FROM projects WHERE id = ?;");
    stmt->bind(1, id);
    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to delete project with ID: " + std::to_string(id));
    }
}

void ProjectRepository::shareProject(uint64_t project_id, uint64_t owner_id,
                                     uint64_t shared_user_id)
{
    std::string role = (owner_id == shared_user_id) ? "owner" : "shared";

    auto stmt = db->prepare("INSERT INTO user_project(project_id, user_id, role) VALUES(?, ?, ?);");
    stmt->bind(1, project_id);
    stmt->bind(2, shared_user_id);
    stmt->bind(3, role);

    if (stmt->step() != silok::db::StepResult::Done)
    {
        throw std::runtime_error("Failed to share project with user ID: " +
                                 std::to_string(shared_user_id));
    }
}

bool ProjectRepository::isProjectShared(uint64_t project_id, uint64_t user_id) const
{
    auto stmt = db->prepare(R"(
            SELECT COUNT(*) FROM user_project
            WHERE project_id = ? AND user_id = ? AND role = 'shared'
        )");
    stmt->bind(1, project_id);
    stmt->bind(2, user_id);
    return stmt->step() == silok::db::StepResult::Row && stmt->columnInt(0) > 0;
}
bool ProjectRepository::isProjectOwned(uint64_t project_id, uint64_t user_id) const
{
    auto stmt = db->prepare(R"(
            SELECT COUNT(*) FROM user_project
            WHERE project_id = ? AND user_id = ? AND role = 'owner'
        )");
    stmt->bind(1, project_id);
    stmt->bind(2, user_id);
    return stmt->step() == silok::db::StepResult::Row && stmt->columnInt(0) > 0;
}
}  // namespace silok::repository
