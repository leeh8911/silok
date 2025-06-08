#pragma once

#include "silok/db/base/base_db_connection.hpp"

namespace silok::repository
{
class UserRepository;
{
 public:
    UserRepository(BaseDBConnectionPtr db_);

    void createUser(const std::string& username);

    std::optional<User> getUserById(uint64_t id);

    void deleteUser(uint64_t id);

 private:
    BaseDBConnectionPtr db{nullptr};
};
}  // namespace silok::repository
