#pragma once

#include "silok/db/base/base_db_connection.hpp"

#include <cstdint>
#include <memory>
#include <string>

#include <sqlite3.h>

#include "silok/db/sql/sqlite_db_statement.hpp"

namespace silok::db
{
class SqliteDBConnection : public BaseDBConnection
{
 public:
    explicit SqliteDBConnection(const std::string& path);
    ~SqliteDBConnection() override = default;

    SqliteDBConnection(const SqliteDBConnection&) = delete;
    SqliteDBConnection& operator=(const SqliteDBConnection&) = delete;

    SqliteDBConnection(SqliteDBConnection&&);
    SqliteDBConnection& operator=(SqliteDBConnection&&);

    void exec(const std::string& sql) override;
    BaseDBStatementPtr prepare(const std::string& sql) override;
    int64_t lastInsertId() override;

 private:
    std::shared_ptr<sqlite3> db{nullptr};
};
}  // namespace silok::db
