#include "silok/db/sql/sqlite_db_connection.hpp"

#include "silok/logger.hpp"

namespace silok::db
{
SqliteDBConnection::SqliteDBConnection(const std::string& path)
{
    sqlite3* raw = nullptr;
    if (sqlite3_open(path.c_str(), &raw) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to open database: {}", sqlite3_errmsg(raw));
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(raw)));
    }
    this->db = std::shared_ptr<sqlite3>(raw,
                                        [](sqlite3* db)
                                        {
                                            if (db)
                                            {
                                                sqlite3_close(db);
                                            }
                                        });
}

SqliteDBConnection::SqliteDBConnection(SqliteDBConnection&& other)
{
    this->db = std::move(other.db);
}

SqliteDBConnection& SqliteDBConnection::operator=(SqliteDBConnection&& other)
{
    if (this != &other)
    {
        this->db = std::move(other.db);
    }
    return *this;
}

void SqliteDBConnection::exec(const std::string& sql)
{
    char* errmsg = nullptr;
    if (sqlite3_exec(this->db.get(), sql.c_str(), nullptr, nullptr, &errmsg) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("SQL execution failed: {}", errmsg);
        sqlite3_free(errmsg);
        throw std::runtime_error("SQL execution failed: " + std::string(errmsg));
    }
}

BaseDBStatementPtr SqliteDBConnection::prepare(const std::string& sql)
{
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(this->db.get(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to prepare statement: {}", sqlite3_errmsg(this->db.get()));
        throw std::runtime_error("Failed to prepare statement: " +
                                 std::string(sqlite3_errmsg(this->db.get())));
    }
    return std::make_shared<SqliteDBStatement>(this->db, sql);
}

int64_t SqliteDBConnection::lastInsertId()
{
    return sqlite3_last_insert_rowid(this->db.get());
}

}  // namespace silok::db
