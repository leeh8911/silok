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
    virtual int64_t Create(const silok::domain::UserProject&);
    virtual std::optional<silok::domain::UserProject> FindById(int64_t id);
    virtual std::vector<silok::domain::UserProject> FindByFirstId(int64_t first_id);
    virtual std::vector<silok::domain::UserProject> FindBySecondId(int64_t second_id);
    virtual bool Update(const silok::domain::UserProject& relation);
    virtual void Delete(int64_t id);
};
}  // namespace silok::infra::repository
