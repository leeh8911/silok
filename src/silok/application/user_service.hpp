#pragma once

#include <optional>
#include <string>

#include "silok/domain/model.hpp"
#include "silok/domain/model_relation.hpp"
#include "silok/domain/repository/base_user_repository.hpp"

namespace silok::application
{
class UserService
{
 public:
    explicit UserService(silok::domain::repository::UserRepositoryPtr user_repository);

    UserService() = default;
    UserService(const UserService&) = default;
    UserService& operator=(const UserService&) = default;
    UserService(UserService&&) = default;
    UserService& operator=(UserService&&) = default;

    static UserService& Get();

    void Create(const std::string& name, const std::string& email, const std::string& password);

    std::optional<std::string> Login(const std::string& email, const std::string& password);

    std::optional<domain::User> FindByEmail(const std::string& email);

    void Update(const domain::User& user);

    void Delete(const domain::User& user);

 private:
    silok::domain::repository::UserRepositoryPtr user_repository{nullptr};
};
}  // namespace silok::application
