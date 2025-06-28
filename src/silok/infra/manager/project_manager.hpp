#pragma once

#include <optional>
#include <string>
#include <vector>

#include "silok/model.hpp"

namespace silok::infra
{
class ProjectManager
{
 public:
    void CreateProject(const User user, const std::string& project_name,
                       std::optional<int64_t> start = std::nullopt,
                       std::optional<int64_t> end = std::nullopt);
    std::vector<Project> GetAllProjects(const User user) const;
    void UpdateProject(const User user, Project project);
    void DeleteProject(const User user, Project project);

 private:
    // Additional private methods or members can be added here
};

}  // namespace silok::infra
