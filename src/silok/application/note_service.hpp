#pragma once

#include <optional>
#include <string>
#include <vector>

#include "silok/domain/data.hpp"
#include "silok/domain/data_relation.hpp"

#include "silok/infra/storage_manager.hpp"

namespace silok::application
{
class NoteService
{
 public:
    static NoteService& Get();

    void Create(const std::string& content, std::string user_token);

    std::vector<domain::Note> FindAll(std::string user_token);

    void Update(const domain::Note& note);

    void Delete(int64_t note_id, std::string user_token);

 private:
    NoteService() = default;
};
}  // namespace silok::application
