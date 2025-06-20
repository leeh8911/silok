#pragma once

#include <optional>
#include <string>

#include "silok/domain/data.hpp"
#include "silok/domain/data_relation.hpp"

namespace silok::application
{
class UserService
{
 public:
    static UserService& Get();

    void Create(const std::string& name, const std::string& email, const std::string& password);

    std::optional<domain::User> FindByEmail(const std::string& email);

    std::optional<std::string> Login(const std::string& email, const std::string& password);

    void Update(const domain::User& user);

    void Delete(const domain::User& user);

 private:
    UserService() = default;
};
}  // namespace silok::application
