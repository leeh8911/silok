#include "silok/db/sql/sqlite_db_statement.hpp"

#include <sqlite3.h>
#include <stdexcept>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/logger.hpp"
namespace silok::db
{
SqliteDBStatement::SqliteDBStatement(std::shared_ptr<sqlite3> db, const std::string& sql)
{
    sqlite3_stmt* raw = nullptr;
    if (sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &raw, nullptr) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to prepare statement: {}", sqlite3_errmsg(db.get()));
        throw std::runtime_error("Failed to prepare statement: " +
                                 std::string(sqlite3_errmsg(db.get())));
    }
    this->stmt.reset(raw);
}

void SqliteDBStatement::bind(int idx, int64_t v)
{
    sqlite3_bind_int64(this->stmt.get(), idx, v);
}

void SqliteDBStatement::bind(int idx, const std::string& v)
{
    sqlite3_bind_text(this->stmt.get(), idx, v.c_str(), -1, SQLITE_TRANSIENT);
}
void SqliteDBStatement::bindNull(int idx)
{
    sqlite3_bind_null(this->stmt.get(), idx);
}
StepResult SqliteDBStatement::step()
{
    int rc = sqlite3_step(this->stmt.get());
    SILOK_LOG_TRACE("SQLite step result: {}", rc);

    switch (rc)
    {
        case SQLITE_ROW:
            return StepResult::Row;
        case SQLITE_DONE:
            return StepResult::Done;
        default:
            // 로깅 등 추가
            return StepResult::Error;
    }
}
int64_t SqliteDBStatement::columnInt(int col) const
{
    return sqlite3_column_int64(this->stmt.get(), col);
}
std::string SqliteDBStatement::columnText(int col) const
{
    auto ptr = reinterpret_cast<const char*>(sqlite3_column_text(this->stmt.get(), col));
    return ptr ? ptr : "";
}

}  // namespace silok::db
