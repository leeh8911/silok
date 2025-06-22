#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "silok/domain/model.hpp"

namespace silok::domain::repository
{
/**
 * @brief Base interface for user repositories.
 *
 * This interface defines the basic operations for user repositories.
 */
class BaseUserRepository
{
 public:
    virtual ~BaseUserRepository() = default;

    virtual int64_t Create(const silok::domain::User& user) = 0;
    virtual std::optional<silok::domain::User> FindByEmail(const std::string& email) = 0;
    virtual std::optional<silok::domain::User> FindById(int64_t id) = 0;
    virtual bool Update(const silok::domain::User& user) = 0;
    virtual void Delete(int64_t id) = 0;
};

using UserRepositoryPtr = std::shared_ptr<BaseUserRepository>;

}  // namespace silok::domain::repository
