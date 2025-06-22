#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/domain/repository/base_note_repository.hpp"

namespace silok::infra::repository
{
class NoteRepository : public silok::domain::repository::BaseNoteRepository
{
 public:
    NoteRepository() = default;
    ~NoteRepository() override = default;

    NoteRepository(const NoteRepository&) = default;
    NoteRepository& operator=(const NoteRepository&) = default;
    NoteRepository(NoteRepository&&) = default;
    NoteRepository& operator=(NoteRepository&&) = default;

    int64_t Create(const silok::domain::Note& note) override;
    std::optional<silok::domain::Note> FindById(int64_t id) override;
    std::vector<silok::domain::Note> FindByIds(const std::vector<int64_t>& ids) override;
    void Update(const silok::domain::Note& note) override;
    void Delete(int64_t id) override;
};

}  // namespace silok::infra::repository
