#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "silok/domain/model_relation.hpp"

namespace silok::domain::repository
{
template <typename RELATION>
class BaseRelationshipRepository
{
 public:
    virtual ~BaseRelationshipRepository() = default;

    virtual int64_t Create(const RELATION& relation) = 0;
    virtual std::optional<RELATION> FindById(int64_t id) = 0;
    virtual std::vector<RELATION> FindByFirstId(int64_t first_id) = 0;
    virtual std::vector<RELATION> FindBySecondId(int64_t second_id) = 0;
    virtual bool Update(const RELATION& relation) = 0;
    virtual void Delete(int64_t id) = 0;
};

using UserNoteRepository = BaseRelationshipRepository<UserNote>;
using UserNoteRepositoryPtr = std::shared_ptr<UserNoteRepository>;

using UserTagRepository = BaseRelationshipRepository<UserTag>;
using UserTagRepositoryPtr = std::shared_ptr<UserTagRepository>;

using UserProjectRepository = BaseRelationshipRepository<UserProject>;
using UserProjectRepositoryPtr = std::shared_ptr<UserProjectRepository>;

using NoteTagRepository = BaseRelationshipRepository<NoteTag>;
using NoteTagRepositoryPtr = std::shared_ptr<NoteTagRepository>;

using NoteProjectRepository = BaseRelationshipRepository<NoteProject>;
using NoteProjectRepositoryPtr = std::shared_ptr<NoteProjectRepository>;

}  // namespace silok::domain::repository
