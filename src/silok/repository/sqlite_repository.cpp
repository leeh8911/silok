#include "silok/repository/sqlite_repository.hpp"
#include "silok/logger.hpp"

#include <sqlite3.h>
#include <chrono>
#include <stdexcept>

namespace silok::repository
{

SqliteRepository::SqliteRepository(const std::string &db_path) : db_path_(db_path)
{
    // 1) DB 열기
    int rc = sqlite3_open(db_path_.c_str(), &this->db);
    if (rc != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Cannot open database: {}", sqlite3_errmsg(this->db));
        throw std::runtime_error("Cannot open database");
    }
    SILOK_LOG_INFO("Database opened successfully: {}", db_path_);

    // 2) notes 테이블 생성 (필드 추가)
    //    - id           : PRIMARY KEY, AUTOINCREMENT
    //    - type         : NoteType (0=Memo,1=Task,2=Event)
    //    - content      : 본문
    //    - created_at   : 생성 시각 (ms 또는 sec 단위 정수)
    //    - updated_at   : 수정 시각
    //    - start_date   : Event 전용 시작 시각 (optional)
    //    - end_date     : Event 전용 종료 시각 (optional)
    //    - done         : Task 전용 완료 여부 (0/1, optional)
    const char *create_sql = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            type        INTEGER NOT NULL,
            content     TEXT    NOT NULL,
            created_at  INTEGER NOT NULL,
            updated_at  INTEGER NOT NULL,
            start_date  INTEGER,
            end_date    INTEGER,
            done        INTEGER
        );
    )";
    char *errmsg = nullptr;
    rc = sqlite3_exec(this->db, create_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to create notes table: {}", errmsg);
        sqlite3_free(errmsg);
        throw std::runtime_error("Failed to create notes table");
    }
}

SqliteRepository::~SqliteRepository()
{
    if (this->db)
    {
        sqlite3_close(this->db);
    }
}
uint64_t SqliteRepository::createNote()
{
    // 1) 현재 시각 (마이크로초 단위)
    auto now_tp = std::chrono::system_clock::now();
    auto now_us =
        std::chrono::duration_cast<std::chrono::microseconds>(now_tp.time_since_epoch()).count();

    // 2) INSERT SQL: type, content, created_at, updated_at, start_date, end_date, done
    const char *sql = R"(
        INSERT INTO notes
            (type, content, created_at, updated_at, start_date, end_date, done)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to prepare INSERT stmt: {}", sqlite3_errmsg(db));
        throw std::runtime_error("Failed to prepare INSERT stmt");
    }

    // 3) 바인딩: 기본은 Memo 타입(0), 빈 문자열, 현재시각, NULL,NULL,NULL
    sqlite3_bind_int64(stmt, 1, static_cast<int64_t>(NoteType::Memo));
    sqlite3_bind_text(stmt, 2, "", -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, now_us);
    sqlite3_bind_int64(stmt, 4, now_us);
    sqlite3_bind_null(stmt, 5);
    sqlite3_bind_null(stmt, 6);
    sqlite3_bind_null(stmt, 7);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        SILOK_LOG_ERROR("Failed to execute INSERT: {}", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute INSERT");
    }
    sqlite3_finalize(stmt);

    return static_cast<uint64_t>(sqlite3_last_insert_rowid(db));
}

::silok::Note SqliteRepository::getNote(uint64_t id)
{
    const char *sql = R"(
        SELECT
          id, type, content,
          created_at, updated_at,
          start_date, end_date, done
        FROM notes
        WHERE id = ?;
    )";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to prepare SELECT stmt: {}", sqlite3_errmsg(db));
        throw std::runtime_error("Failed to prepare SELECT stmt");
    }
    sqlite3_bind_int64(stmt, 1, static_cast<int64_t>(id));

    ::silok::Note note{};
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        note.id = sqlite3_column_int64(stmt, 0);
        note.type = static_cast<NoteType>(sqlite3_column_int64(stmt, 1));
        note.content = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

        // 마이크로초 → time_point
        auto ca = sqlite3_column_int64(stmt, 3);
        auto ua = sqlite3_column_int64(stmt, 4);
        note.created_at = std::chrono::system_clock::time_point{std::chrono::microseconds{ca}};
        note.updated_at = std::chrono::system_clock::time_point{std::chrono::microseconds{ua}};

        // start_date(optional)
        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL)
        {
            auto sd = sqlite3_column_int64(stmt, 5);
            note.start_date = std::chrono::system_clock::time_point{std::chrono::microseconds{sd}};
        }

        // end_date(optional)
        if (sqlite3_column_type(stmt, 6) != SQLITE_NULL)
        {
            auto ed = sqlite3_column_int64(stmt, 6);
            note.end_date = std::chrono::system_clock::time_point{std::chrono::microseconds{ed}};
        }

        // done(optional)
        if (sqlite3_column_type(stmt, 7) != SQLITE_NULL)
        {
            note.done = (sqlite3_column_int(stmt, 7) != 0);
        }
    }
    else
    {
        sqlite3_finalize(stmt);
        SILOK_LOG_ERROR("Note with ID {} not found", id);
        throw std::runtime_error("Note not found");
    }
    sqlite3_finalize(stmt);
    return note;
}

bool SqliteRepository::updateNote(const Note &note)
{
    // 1) 현재 시각
    auto now_tp = std::chrono::system_clock::now();
    auto now_us =
        std::chrono::duration_cast<std::chrono::microseconds>(now_tp.time_since_epoch()).count();

    // 2) UPDATE 모든 컬럼
    const char *sql = R"(
        UPDATE notes SET
            type        = ?,
            content     = ?,
            updated_at  = ?,
            start_date  = ?,
            end_date    = ?,
            done        = ?
        WHERE id = ?;
    )";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to prepare UPDATE stmt: {}", sqlite3_errmsg(db));
        throw std::runtime_error("Failed to prepare UPDATE stmt");
    }

    // 3) 바인딩
    sqlite3_bind_int64(stmt, 1, static_cast<int64_t>(note.type));
    sqlite3_bind_text(stmt, 2, note.content.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 3, now_us);

    if (note.start_date)
    {
        auto sd = std::chrono::duration_cast<std::chrono::microseconds>(
                      note.start_date->time_since_epoch())
                      .count();
        sqlite3_bind_int64(stmt, 4, sd);
    }
    else
    {
        sqlite3_bind_null(stmt, 4);
    }

    if (note.end_date)
    {
        auto ed =
            std::chrono::duration_cast<std::chrono::microseconds>(note.end_date->time_since_epoch())
                .count();
        sqlite3_bind_int64(stmt, 5, ed);
    }
    else
    {
        sqlite3_bind_null(stmt, 5);
    }

    if (note.done)
    {
        sqlite3_bind_int(stmt, 6, *note.done ? 1 : 0);
    }
    else
    {
        sqlite3_bind_null(stmt, 6);
    }

    sqlite3_bind_int64(stmt, 7, static_cast<int64_t>(note.id));

    // 4) 실행 & 정리
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        SILOK_LOG_ERROR("Failed to execute UPDATE: {}", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    return sqlite3_changes(db) > 0;
}

bool SqliteRepository::deleteNote(uint64_t id)
{
    const char *sql = R"(
        DELETE FROM notes
         WHERE id = ?;
    )";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        SILOK_LOG_ERROR("Failed to prepare DELETE stmt: {}", sqlite3_errmsg(this->db));
        throw std::runtime_error("Failed to prepare DELETE stmt");
    }
    sqlite3_bind_int64(stmt, 1, static_cast<int64_t>(id));

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        SILOK_LOG_ERROR("Failed to execute DELETE: {}", sqlite3_errmsg(this->db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    return sqlite3_changes(this->db) > 0;
}

}  // namespace silok::repository
