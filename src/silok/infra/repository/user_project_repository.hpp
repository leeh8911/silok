#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/repository/base_relationship_repository.hpp"

namespace silok::infra::repository
{
class UserProjectRepository : public silok::domain::repository::UserProjectRepository
{
 public:
    virtual int64_t Create(const UserProject&);
    virtual std::optional<UserProject> FindById(int64_t id);
    virtual std::vector<UserProject> FindByFirstId(int64_t first_id);
    virtual std::vector<UserProject> FindBySecondId(int64_t second_id);
    virtual bool Update(const UserProject& relation);
    virtual void Delete(int64_t id);
};
}  // namespace silok::infra::repository
