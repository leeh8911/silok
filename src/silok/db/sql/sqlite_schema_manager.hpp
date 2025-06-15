#pragma once

#include "silok/db/base/base_schema_manager.hpp"

namespace silok::db
{
class SqliteSchemaManager : public BaseSchemaManager
{
 public:
    void migrate(BaseDBConnectionPtr db) override
    {
        // users, projects, tags, notes
        db->exec(R"(
      CREATE TABLE IF NOT EXISTS users (
        id       INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT    NOT NULL UNIQUE
      );
      CREATE TABLE IF NOT EXISTS projects (
        id          INTEGER PRIMARY KEY AUTOINCREMENT,
        name        TEXT    NOT NULL
      );
      CREATE TABLE IF NOT EXISTS tags (
        id    INTEGER PRIMARY KEY AUTOINCREMENT,
        name  TEXT    NOT NULL UNIQUE
      );
      CREATE TABLE IF NOT EXISTS notes (
        id          INTEGER PRIMARY KEY AUTOINCREMENT,
        content     TEXT,
        created_at  INTEGER NOT NULL,
        updated_at  INTEGER NOT NULL
      );
    )");

        // user_project, user_tag, user_note
        db->exec(R"(
      CREATE TABLE IF NOT EXISTS user_project (
        user_id    INTEGER NOT NULL REFERENCES users(id)    ON DELETE CASCADE,
        project_id INTEGER NOT NULL REFERENCES projects(id) ON DELETE CASCADE,
        role       TEXT    NOT NULL CHECK(role IN('owner','shared')),
        PRIMARY KEY(user_id, project_id)
      );
      CREATE TABLE IF NOT EXISTS user_tag (
        user_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
        tag_id  INTEGER NOT NULL REFERENCES tags(id) ON DELETE CASCADE,
        role    TEXT    NOT NULL CHECK(role IN('owner','shared')),
        PRIMARY KEY(user_id, tag_id)
      );
      CREATE TABLE IF NOT EXISTS user_note (
        user_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
        note_id INTEGER NOT NULL REFERENCES notes(id) ON DELETE CASCADE,
        role    TEXT    NOT NULL CHECK(role IN('owner','shared')),
        PRIMARY KEY(user_id, note_id)
      );
    )");

        // note_project, note_tag
        db->exec(R"(
      CREATE TABLE IF NOT EXISTS note_project (
        note_id    INTEGER NOT NULL REFERENCES notes(id)    ON DELETE CASCADE,
        project_id INTEGER NOT NULL REFERENCES projects(id) ON DELETE CASCADE,
        PRIMARY KEY(note_id, project_id)
      );
      CREATE TABLE IF NOT EXISTS note_tag (
        note_id INTEGER NOT NULL REFERENCES notes(id) ON DELETE CASCADE,
        tag_id  INTEGER NOT NULL REFERENCES tags(id) ON DELETE CASCADE,
        PRIMARY KEY(note_id, tag_id)
      );
    )");
    }
};
}  // namespace silok::db
