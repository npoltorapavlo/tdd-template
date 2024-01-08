#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sqlite3.h>

using ::testing::Eq;
using ::testing::NotNull;
using ::testing::Test;

constexpr const char* Path = "/tmp/sqlitetest";

// If tests operate on similar data use a fixture
class ADb : public Test {
protected:
    sqlite3* db;
    virtual ~ADb() override = default;
    virtual void SetUp() override
    {
        // ASSERT_* fails the test case and evaluates to return;
        ASSERT_THAT(sqlite3_open(Path, &db), Eq(SQLITE_OK));
        // Assertions read left to right
        ASSERT_THAT(db, NotNull());
        ASSERT_THAT(sqlite3_exec(db,
                        "CREATE TABLE if not exists item (key TEXT,value TEXT,UNIQUE(key) ON CONFLICT REPLACE);",
                        nullptr, nullptr, nullptr),
            Eq(SQLITE_OK));
        ASSERT_THAT(sqlite3_exec(db, "DELETE FROM item;", nullptr, nullptr, nullptr), Eq(SQLITE_OK));
    }
    virtual void TearDown() override
    {
        if (db) {
            EXPECT_THAT(sqlite3_close_v2(db), Eq(SQLITE_OK));
        }
    }
};

// The name left to right reveals a sentence that describes what is verified
TEST_F(ADb, DoesNotHaveItems)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM item;", -1, &stmt, nullptr);
    // One Assert per Test
    EXPECT_THAT(sqlite3_step(stmt), Eq(SQLITE_DONE));
    sqlite3_finalize(stmt);
}

// The fixture name clearly describes the context
class ADbWithItems : public ADb {
protected:
    virtual ~ADbWithItems() override = default;
    virtual void SetUp() override
    {
        ADb::SetUp();
        ASSERT_THAT(sqlite3_exec(db, "INSERT INTO item (key,value) values ('k1','v1');", nullptr, nullptr, nullptr), Eq(SQLITE_OK));
        ASSERT_THAT(sqlite3_exec(db, "INSERT INTO item (key,value) values ('k2','v2');", nullptr, nullptr, nullptr), Eq(SQLITE_OK));
    }
};

// Tests should be fast, independent and repeatable
TEST_F(ADbWithItems, HasCount2)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT count(*) FROM item;", -1, &stmt, nullptr);
    EXPECT_THAT(sqlite3_step(stmt), Eq(SQLITE_ROW));
    EXPECT_THAT(sqlite3_column_int(stmt, 0), Eq(2));
    sqlite3_finalize(stmt);
}

class ADbWithItemsAndLimit : public ADbWithItems {
protected:
    virtual ~ADbWithItemsAndLimit() override = default;
    virtual void SetUp() override
    {
        ADbWithItems::SetUp();
        ASSERT_THAT(sqlite3_exec(db,
                        "CREATE TEMPORARY TRIGGER itemtrigger INSERT ON item"
                        " BEGIN"
                        " SELECT CASE"
                        " WHEN (SELECT sum(length(key)+length(value))+length(new.key)+length(new.value) FROM item) > 10"
                        " THEN RAISE (FAIL, 'db full')"
                        " END;"
                        "END;",
                        nullptr, nullptr, nullptr),
            Eq(SQLITE_OK));
    }
};

TEST_F(ADbWithItemsAndLimit, DoesNotInsertWhenReachesLimit)
{
    // Setup
    EXPECT_THAT(sqlite3_exec(db, "INSERT INTO item (key,value) values ('a','b');", nullptr, nullptr, nullptr), Eq(SQLITE_OK));

    // Execute-Verify
    EXPECT_THAT(sqlite3_exec(db, "INSERT INTO item (key,value) values ('c','d');", nullptr, nullptr, nullptr), Eq(SQLITE_CONSTRAINT));
}

TEST_F(ADbWithItemsAndLimit, InsertsAfterReachesLimitAndDeletesItems)
{
    // Setup
    EXPECT_THAT(sqlite3_exec(db, "INSERT INTO item (key,value) values ('k3','v3');", nullptr, nullptr, nullptr), Eq(SQLITE_CONSTRAINT));
    EXPECT_THAT(sqlite3_exec(db, "DELETE FROM item;", nullptr, nullptr, nullptr), Eq(SQLITE_OK));

    // Execute-Verify
    EXPECT_THAT(sqlite3_exec(db, "INSERT INTO item (key,value) values ('k3','v3');", nullptr, nullptr, nullptr), Eq(SQLITE_OK));
}
