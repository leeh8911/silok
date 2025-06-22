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
class NoteProjectRepository : public silok::domain::repository::NoteProjectRepository
{
 public:
    virtual int64_t Create(const silok::domain::NoteProject&);
    virtual std::optional<silok::domain::NoteProject> FindById(int64_t id);
    virtual std::vector<silok::domain::NoteProject> FindByFirstId(int64_t first_id);
    virtual std::vector<silok::domain::NoteProject> FindBySecondId(int64_t second_id);
    virtual bool Update(const silok::domain::NoteProject& relation);
    virtual void Delete(int64_t id);
};
}  // namespace silok::infra::repository
