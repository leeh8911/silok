#pragma once

#include "silok/db/base/base_db_statement.hpp"

#include <memory>
#include <string>

#include <sqlite3.h>

namespace silok::db
{
class SqliteDBStatement : public BaseDBStatement
{
 public:
    SqliteDBStatement(std::shared_ptr<sqlite3> db, const std::string& sql);
    ~SqliteDBStatement() override = default;
    void bind(int idx, int64_t v) override;
    void bind(int idx, const std::string& v) override;
    void bindNull(int idx) override;
    StepResult step() override;
    int64_t columnInt(int col) const override;
    std::string columnText(int col) const override;

 private:
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt{nullptr, &sqlite3_finalize};
};
}  // namespace silok::db
