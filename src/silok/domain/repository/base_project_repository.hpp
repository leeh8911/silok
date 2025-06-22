#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "silok/domain/model.hpp"

namespace silok::domain::repository
{
class BaseProjectRepository
{
 public:
    virtual ~BaseProjectRepository() = default;

    virtual int64_t Create(const silok::domain::Project& tag) = 0;
    virtual std::optional<silok::domain::Project> FindById(int64_t id) = 0;
    virtual std::vector<silok::domain::Project> FindByIds(const std::vector<int64_t>& ids) = 0;
    virtual void Update(const silok::domain::Project& tag) = 0;
    virtual void Delete(int64_t id) = 0;
};

using ProjectRepositoryPtr = std::shared_ptr<BaseProjectRepository>;

}  // namespace silok::domain::repository
