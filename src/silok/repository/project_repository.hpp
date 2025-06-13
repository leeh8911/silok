#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "silok/data/project.hpp"
#include "silok/db/base/base_db_connection.hpp"

namespace silok::repository
{
class ProjectRepository
{
 public:
    explicit ProjectRepository(silok::db::BaseDBConnectionPtr db_);

    void createProject(const std::string& name, uint64_t owner_id);

    std::vector<Project> getProjectsByOwner(uint64_t owner_id);

    std::optional<Project> getProjectById(uint64_t id, uint64_t owner_id);

    void deleteProject(uint64_t id, uint64_t owner_id);

    void shareProject(uint64_t project_id, uint64_t owner_id, uint64_t shared_user_id);

    bool isProjectShared(uint64_t project_id, uint64_t user_id) const;
    bool isProjectOwned(uint64_t project_id, uint64_t user_id) const;

 private:
    silok::db::BaseDBConnectionPtr db{nullptr};
};

}  // namespace silok::repository
