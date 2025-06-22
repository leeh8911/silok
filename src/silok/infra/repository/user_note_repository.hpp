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
class UserNoteRepository : public silok::domain::repository::UserNoteRepository
{
 public:
    virtual int64_t Create(const UserNote&);
    virtual std::optional<UserNote> FindById(int64_t id);
    virtual std::vector<UserNote> FindByFirstId(int64_t first_id);
    virtual std::vector<UserNote> FindBySecondId(int64_t second_id);
    virtual bool Update(const UserNote& relation);
    virtual void Delete(int64_t id);
};
}  // namespace silok::infra::repository
