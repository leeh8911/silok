#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "silok/domain/model.hpp"

namespace silok::domain::repository
{
class BaseNoteRepository
{
 public:
    virtual ~BaseNoteRepository() = default;

    virtual int64_t Create(const silok::domain::Note& note) = 0;
    virtual std::optional<silok::domain::Note> FindById(int64_t id) = 0;
    virtual std::vector<silok::domain::Note> FindByIds(const std::vector<int64_t>& ids) = 0;
    virtual bool Update(const silok::domain::Note& note) = 0;
    virtual void Delete(int64_t id) = 0;
};

using NoteRepositoryPtr = std::shared_ptr<BaseNoteRepository>;

}  // namespace silok::domain::repository
