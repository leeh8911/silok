#pragma once

#include <optional>
#include <string>
#include <vector>

#include "silok/domain/data.hpp"
#include "silok/domain/data_relation.hpp"
#include "silok/infra/storage_manager.hpp"

namespace silok::application
{
class TagService
{
 public:
    static TagService& Get();

    void Create(const std::string& name, std::string user_token,
                const std::optional<domain::Note>& = std::nullopt);

    std::vector<domain::Tag> FindAll(std::string user_token);

    void Update(const domain::Tag& tag, const std::optional<domain::Note>& = std::nullopt);

    void Delete(int64_t tag_id, std::string user_token);

 private:
    TagService() = default;
};
}  // namespace silok::application
