#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/db/sql/sqlite_db_statement.hpp"
#include "silok/db/sql/sqlite_schema_manager.hpp"

class SqliteDBConnectionTest : public ::testing::Test
{
 protected:
    ::silok::db::SqliteDBConnection db{":memory:"};

    void SetUp() override
    {
        // Migrate schema into an in-memory database
        ::silok::db::SqliteSchemaManager schemaManager;
        schemaManager.migrate(db);
    }

    void TearDown() override
    {
        // No cleanup needed for in-memory DB
    }
};

// Test exec() and lastInsertId()
TEST_F(SqliteDBConnectionTest, ExecAndLastInsertId)
{
    // Insert two users via exec()
    db.exec("INSERT INTO users(username) VALUES('alice');");
    int64_t firstId = db.lastInsertId();
    EXPECT_GT(firstId, 0);

    db.exec("INSERT INTO users(username) VALUES('bob');");
    int64_t secondId = db.lastInsertId();
    EXPECT_EQ(secondId, firstId + 1);
}

// Test prepare(), bind(), step(), and column\* for INSERT and SELECT
TEST_F(SqliteDBConnectionTest, PrepareBindStepColumn)
{
    // INSERT using prepared statement
    auto insertStmt = db.prepare("INSERT INTO users(username) VALUES(?);");
    insertStmt->bind(1, std::string("charlie"));
    // INSERT returns SQLITE\_DONE => step() should return false
    EXPECT_FALSE(insertStmt->step());
    int64_t newUserId = db.lastInsertId();
    EXPECT_GT(newUserId, 0);

    // SELECT using prepared statement
    auto selectStmt = db.prepare("SELECT username FROM users WHERE id = ?;");
    selectStmt->bind(1, newUserId);
    // First row should be available
    ASSERT_TRUE(selectStmt->step());
    std::string username = selectStmt->columnText(0);
    EXPECT_EQ(username, "charlie");
    // No more rows
    EXPECT_FALSE(selectStmt->step());
}

// Test that schema manager created all tables and relationships
TEST_F(SqliteDBConnectionTest, SchemaCreatesAndRelatesTables)
{
    // Insert into projects
    db.exec("INSERT INTO projects(name) VALUES('ProjA');");
    int64_t projId = db.lastInsertId();
    EXPECT_GT(projId, 0);

    // Insert into tags
    db.exec("INSERT INTO tags(name) VALUES('urgent');");
    int64_t tagId = db.lastInsertId();
    EXPECT_GT(tagId, 0);

    // Insert into notes
    db.exec("INSERT INTO notes(content, created_at, updated_at) VALUES('Note1', 1, 1);");
    int64_t noteId = db.lastInsertId();
    EXPECT_GT(noteId, 0);

    // Link note to project
    std::ostringstream npSql;
    npSql << "INSERT INTO note_project(note_id, project_id) VALUES(" << noteId << ", " << projId
          << ");";
    db.exec(npSql.str());

    // Verify note_project relationship
    auto npStmt = db.prepare("SELECT project_id FROM note_project WHERE note_id = ?;");
    npStmt->bind(1, noteId);
    ASSERT_TRUE(npStmt->step());
    EXPECT_EQ(npStmt->columnInt(0), projId);
    EXPECT_FALSE(npStmt->step());

    // Link note to tag
    std::ostringstream ntSql;
    ntSql << "INSERT INTO note_tag(note_id, tag_id) VALUES(" << noteId << ", " << tagId << ");";
    db.exec(ntSql.str());

    // Verify note_tag relationship
    auto ntStmt = db.prepare("SELECT tag_id FROM note_tag WHERE note_id = ?;");
    ntStmt->bind(1, noteId);
    ASSERT_TRUE(ntStmt->step());
    EXPECT_EQ(ntStmt->columnInt(0), tagId);
    EXPECT_FALSE(ntStmt->step());
}
