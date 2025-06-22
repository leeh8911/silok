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
class NoteTagRepository : public silok::domain::repository::NoteTagRepository
{
 public:
    virtual int64_t Create(const silok::domain::NoteTag&);
    virtual std::optional<silok::domain::NoteTag> FindById(int64_t id);
    virtual std::vector<silok::domain::NoteTag> FindByFirstId(int64_t first_id);
    virtual std::vector<silok::domain::NoteTag> FindBySecondId(int64_t second_id);
    virtual bool Update(const silok::domain::NoteTag& relation);
    virtual void Delete(int64_t id);
};
}  // namespace silok::infra::repository
