#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/domain/repository/base_project_repository.hpp"

namespace silok::infra::repository
{
class ProjectRepository : public silok::domain::repository::BaseProjectRepository
{
 public:
    ProjectRepository() = default;
    ~ProjectRepository() override = default;

    ProjectRepository(const ProjectRepository&) = default;
    ProjectRepository& operator=(const ProjectRepository&) = default;
    ProjectRepository(ProjectRepository&&) = default;
    ProjectRepository& operator=(ProjectRepository&&) = default;

    int64_t Create(const silok::domain::Project& project) override;
    std::optional<silok::domain::Project> FindById(int64_t id) override;
    std::vector<silok::domain::Project> FindByIds(const std::vector<int64_t>& ids) override;
    void Update(const silok::domain::Project& project) override;
    void Delete(int64_t id) override;
};

}  // namespace silok::infra::repository
