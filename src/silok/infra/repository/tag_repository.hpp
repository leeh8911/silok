#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "silok/domain/model.hpp"
#include "silok/domain/repository/base_tag_repository.hpp"

namespace silok::infra::repository
{
class TagRepository : public silok::domain::repository::BaseTagRepository
{
 public:
    TagRepository() = default;
    ~TagRepository() override = default;

    TagRepository(const TagRepository&) = default;
    TagRepository& operator=(const TagRepository&) = default;
    TagRepository(TagRepository&&) = default;
    TagRepository& operator=(TagRepository&&) = default;

    int64_t Create(const silok::domain::Tag& tag) override;
    std::optional<silok::domain::Tag> FindById(int64_t id) override;
    std::vector<silok::domain::Tag> FindByIds(const std::vector<int64_t>& ids) override;
    bool Update(const silok::domain::Tag& tag) override;
    void Delete(int64_t id) override;
};

}  // namespace silok::infra::repository
