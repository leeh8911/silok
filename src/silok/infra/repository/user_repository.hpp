#pragma once

#include <cstdint>
#include <string>

#include "silok/domain/model.hpp"
#include "silok/domain/repository/base_user_repository.hpp"

namespace silok::infra::repository
{

class UserRepository : public silok::domain::repository::BaseUserRepository
{
 public:
    ~UserRepository() override = default;

    UserRepository();
    UserRepository(const UserRepository&) = default;
    UserRepository& operator=(const UserRepository&) = default;
    UserRepository(UserRepository&&) = default;
    UserRepository& operator=(UserRepository&&) = default;

    int64_t Create(const silok::domain::User& user) override;
    std::optional<silok::domain::User> FindByEmail(const std::string& email) override;
    std::optional<silok::domain::User> FindById(int64_t id) override;
    bool Update(const silok::domain::User& user) override;
    void Delete(int64_t id) override;
};
}  // namespace silok::infra::repository
