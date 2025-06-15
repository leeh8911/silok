
#include <gtest/gtest.h>
#include <odb/sqlite/database.hxx>
#include <odb/transaction.hxx>

#include "model-odb.hxx"

class ODBTest : public ::testing::Test
{
 protected:
    std::unique_ptr<odb::sqlite::database> db;

    void SetUp() override
    {
        db = std::make_unique<odb::sqlite::database>(":memory:",
                                                     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    }

    void TearDown() override
    {
        db.reset();
    }
};

// ✅ 유저 생성 및 조회
TEST_F(ODBTest, CreateUserAndLoad)
{
    {
        odb::transaction t(db->begin());
        User* u(new User("alice"));
        db->persist(u);
        t.commit();
    }

    {
        odb::transaction t(db->begin());
        User* loaded = db->load<User>(1);
        EXPECT_EQ(loaded->username, "alice");
        t.commit();
    }
}

// ✅ 노트 생성 및 조회
TEST_F(ODBTest, CreateNoteAndLoad)
{
    unsigned long created = 1234567890;
    unsigned long updated = 1234567999;

    {
        odb::transaction t(db->begin());
        Note* n(new Note("test note", created, updated));
        db->persist(n);
        t.commit();
    }

    {
        odb::transaction t(db->begin());
        Note* loaded = db->load<Note>(1);
        EXPECT_EQ(loaded->content, "test note");
        EXPECT_EQ(loaded->created_at, created);
        EXPECT_EQ(loaded->updated_at, updated);
        t.commit();
    }
}

// ✅ 유저 + 노트 복합 생성 및 count 확인
TEST_F(ODBTest, UserNoteMultipleInsertCount)
{
    {
        odb::transaction t(db->begin());
        db->persist(new User("bob"));
        db->persist(new User("carol"));
        db->persist(new Note("note1", 1, 2));
        db->persist(new Note("note2", 2, 3));
        t.commit();
    }

    {
        odb::transaction t(db->begin());
        odb::result<User> users = db->query<User>();
        odb::result<Note> notes = db->query<Note>();
        EXPECT_EQ(users.size(), 2);
        EXPECT_EQ(notes.size(), 2);
        t.commit();
    }
}
