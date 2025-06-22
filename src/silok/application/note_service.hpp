#pragma once

#include <optional>
#include <string>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/repository/base_note_repository.hpp"
#include "silok/domain/repository/base_project_repository.hpp"
#include "silok/domain/repository/base_relationship_repository.hpp"
#include "silok/domain/repository/base_tag_repository.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::application
{
class NoteService
{
 public:
    void Create(const std::string& content, std::string user_token);

    std::vector<domain::Note> FindAll(std::string user_token);

    void Update(const domain::Note& note);

    void AddTag(int64_t note_id, int64_t tag_id, std::string user_token);
    void RemoveTag(int64_t note_id, int64_t tag_id, std::string user_token);
    void AssignProject(int64_t note_id, int64_t project_id, std::string user_token);
    void UnassignProject(int64_t note_id, int64_t project_id, std::string user_token);

    void Delete(int64_t note_id, std::string user_token);

 private:
    NoteService() = default;

    NoteRepositoryPtr note_repository{nullptr};
    TagRepositoryPtr tag_repository{nullptr};
    ProjectRepositoryPtr project_repository{nullptr};
    UserNoteRepositoryPtr user_note_repository{nullptr};
    UserTagRepositoryPtr user_tag_repository{nullptr};
    UserProjectRepositoryPtr user_project_repository{nullptr};
    NoteTagRepositoryPtr note_tag_repository{nullptr};
    NoteProjectRepositoryPtr note_project_repository{nullptr};
};
}  // namespace silok::application
