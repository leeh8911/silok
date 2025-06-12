#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "silok/db/sql/sqlite_db_connection.hpp"
#include "silok/db/sql/sqlite_db_statement.hpp"
#include "silok/db/sql/sqlite_schema_manager.hpp"

class SqliteDBConnectionTest : public ::testing::Test
{
 protected:
    silok::db::SqliteDBConnectionPtr db{nullptr};

    void SetUp() override
    {
        // Migrate schema into an in-memory database
        db = std::make_shared<silok::db::SqliteDBConnection>(":memory:");

        silok::db::SqliteSchemaManager schemaManager;
        schemaManager.migrate(*db);
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
    db->exec("INSERT INTO users(username) VALUES('alice');");
    int64_t firstId = db->lastInsertId();
    EXPECT_GT(firstId, 0);

    db->exec("INSERT INTO users(username) VALUES('bob');");
    int64_t secondId = db->lastInsertId();
    EXPECT_EQ(secondId, firstId + 1);
}

// Test prepare(), bind(), step(), and column\* for INSERT and SELECT
TEST_F(SqliteDBConnectionTest, PrepareBindStepColumn)
{
    // INSERT using prepared statement
    auto insertStmt = db->prepare("INSERT INTO users(username) VALUES(?);");
    insertStmt->bind(1, std::string("charlie"));
    // Expect Done
    EXPECT_EQ(insertStmt->step(), silok::db::StepResult::Done);

    int64_t newUserId = db->lastInsertId();
    EXPECT_GT(newUserId, 0);

    // SELECT using prepared statement
    auto selectStmt = db->prepare("SELECT username FROM users WHERE id = ?;");
    selectStmt->bind(1, newUserId);

    // Expect Row
    ASSERT_EQ(selectStmt->step(), silok::db::StepResult::Row);
    std::string username = selectStmt->columnText(0);
    EXPECT_EQ(username, "charlie");

    // No more rows → should be Done
    EXPECT_EQ(selectStmt->step(), silok::db::StepResult::Done);
}

// Test that schema manager created all tables and relationships
TEST_F(SqliteDBConnectionTest, SchemaCreatesAndRelatesTables)
{
    // Insert into projects
    db->exec("INSERT INTO projects(name) VALUES('ProjA');");
    int64_t projId = db->lastInsertId();
    EXPECT_GT(projId, 0);

    // Insert into tags
    db->exec("INSERT INTO tags(name) VALUES('urgent');");
    int64_t tagId = db->lastInsertId();
    EXPECT_GT(tagId, 0);

    // Insert into notes
    db->exec("INSERT INTO notes(content, created_at, updated_at) VALUES('Note1', 1, 1);");
    int64_t noteId = db->lastInsertId();
    EXPECT_GT(noteId, 0);

    // Link note to project
    std::ostringstream npSql;
    npSql << "INSERT INTO note_project(note_id, project_id) VALUES(" << noteId << ", " << projId
          << ");";
    db->exec(npSql.str());

    // Verify note_project relationship
    auto npStmt = db->prepare("SELECT project_id FROM note_project WHERE note_id = ?;");
    npStmt->bind(1, noteId);
    ASSERT_EQ(npStmt->step(), silok::db::StepResult::Row);
    EXPECT_EQ(npStmt->columnInt(0), projId);
    EXPECT_EQ(npStmt->step(), silok::db::StepResult::Done);

    // Link note to tag
    std::ostringstream ntSql;
    ntSql << "INSERT INTO note_tag(note_id, tag_id) VALUES(" << noteId << ", " << tagId << ");";
    db->exec(ntSql.str());

    // Verify note_tag relationship
    auto ntStmt = db->prepare("SELECT tag_id FROM note_tag WHERE note_id = ?;");
    ntStmt->bind(1, noteId);
    ASSERT_EQ(ntStmt->step(), silok::db::StepResult::Row);
    EXPECT_EQ(ntStmt->columnInt(0), tagId);
    EXPECT_EQ(ntStmt->step(), silok::db::StepResult::Done);
}
