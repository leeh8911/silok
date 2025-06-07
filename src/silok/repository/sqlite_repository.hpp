#pragma once

#include "silok/repository/base_repository.hpp"

#include <sqlite3.h>

namespace silok::repository
{
class SqliteRepository : public BaseRepository
{
 public:
    SqliteRepository(const std::string &db_path);
    ~SqliteRepository() override;

    uint64_t createNote() override;
    ::silok::Note getNote(uint64_t id) override;
    bool updateNote(const Note &note) override;
    bool deleteNote(uint64_t id) override;

 private:
    sqlite3 *db{nullptr};
    sqlite3_stmt *stmt{nullptr};

    std::string db_path_{};
};
}  // namespace silok::repository
