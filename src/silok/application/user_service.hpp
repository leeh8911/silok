#pragma once

#include <optional>
#include <string>

#include "silok/domain/user.hpp"

namespace silok::application
{
class UserService
{
 public:
    static UserService& Get();

    void Create(const std::string& name, const std::string& email, const std::string& password);

    std::optional<domain::User> FindByEmail(const std::string& email);

 private:
    UserService() = default;
};
}  // namespace silok::application
