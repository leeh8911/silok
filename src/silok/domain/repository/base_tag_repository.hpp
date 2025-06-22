#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "silok/domain/model.hpp"

namespace silok::domain::repository
{
class BaseTagRepository
{
 public:
    virtual ~BaseTagRepository() = default;

    virtual int64_t Create(const silok::domain::Tag& tag) = 0;
    virtual std::optional<silok::domain::Tag> FindById(int64_t id) = 0;
    virtual std::vector<silok::domain::Tag> FindByIds(const std::vector<int64_t>& ids) = 0;
    virtual void Update(const silok::domain::Tag& tag) = 0;
    virtual void Delete(int64_t id) = 0;
};

using TagRepositoryPtr = std::shared_ptr<BaseTagRepository>;

}  // namespace silok::domain::repository
