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
class UserTagRepository : public silok::domain::repository::UserTagRepository
{
 public:
    virtual int64_t Create(const silok::domain::UserTag&);
    virtual std::optional<silok::domain::UserTag> FindById(int64_t id);
    virtual std::vector<silok::domain::UserTag> FindByFirstId(int64_t first_id);
    virtual std::vector<silok::domain::UserTag> FindBySecondId(int64_t second_id);
    virtual bool Update(const silok::domain::UserTag& relation);
    virtual void Delete(int64_t id);
};
}  // namespace silok::infra::repository
