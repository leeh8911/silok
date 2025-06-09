#pragma once

#include <optional>
#include <string>

#include "silok/data/user.hpp"
#include "silok/db/base/base_db_connection.hpp"

namespace silok::repository
{
class UserRepository
{
 public:
    UserRepository(silok::db::BaseDBConnectionPtr db_);

    void createUser(const std::string& username);

    std::optional<User> getUserById(uint64_t id);

    std::optional<User> getUserByName(const std::string& username);

    void deleteUser(uint64_t id);

 private:
    silok::db::BaseDBConnectionPtr db{nullptr};
};
}  // namespace silok::repository
