#pragma once

#include "silok/db/base/base_db_connection.hpp"
#include "silok/repository/base_repository.hpp"

namespace silok::repository
{

class UserRepository : public BaseRepository
{
 public:
    explicit UserRepository(silok::db::BaseDBConnectionPtr db);
    ~UserRepository() override = default;

    void create() override;                  // Create a new user
    void read(int64_t id) override;          // Read a user by ID
    void update(int64_t id) override;        // Update a user by ID
    void deleteRecord(int64_t id) override;  // Delete a user by ID

 private:
    silok::db::BaseDBConnectionPtr db;
};

}  // namespace silok::repository
