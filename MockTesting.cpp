#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class DBConnection
{
public:
    DBConnection(/* args */);
    virtual ~DBConnection();

    virtual bool open();
    virtual bool close();
    virtual bool execQuery(char* query);
};

class ThatUsesDB
{
private:
    DBConnection* connect;

public:
    ThatUsesDB(/* args */);
    ~ThatUsesDB();

    void openConnection();
    void useConnection(char* query);
    void closeConnection();
};

void ThatUsesDB::openConnection()
{
    connect->open();
}
void ThatUsesDB::useConnection(char* query)
{
    connect->execQuery(query);
}
void ThatUsesDB::closeConnection()
{
    connect->close();
}

void CreateDBConnection(DBConnection* DBCreater) {
    char* queue;
    DBCreater->open();
    DBCreater->execQuery(queue);
    DBCreater->close();
}

class MockCreater : public DBConnection {
public:
    // пользуемся макросами GTest для определения мок-методов в объекте
    MOCK_METHOD(bool, open, (), (override));
    MOCK_METHOD(bool, close, (), (override));
    MOCK_METHOD(bool, execQuery, (char* queue), (override));    
};

TEST(CreaterTest, test1) {
    MockCreater createDB;
    EXPECT_CALL(createDB, open).Times(::testing::AtLeast(1));
    EXPECT_CALL(createDB, close).WillOnce(::testing::Return(open));
    EXPECT_CALL(createDB, execQuery).Times(::testing::AtLeast(1));
    
    CreateDBConnection(&createDB);
}



void CreateThatUsesDB(ThatUsesDB* DBUse) {
    char* queue;
    DBUse->openConnection();
    DBUse->useConnection(queue);
    DBUse->closeConnection();
}

class MockUser : public ThatUsesDB {
public:
    // пользуемся макросами GTest для определения мок-методов в объекте
    MOCK_METHOD(bool, open, ());
    MOCK_METHOD(bool, close, ());
    MOCK_METHOD(bool, execQuery, (char* queue));
};

TEST(UserTest, test1) {
    MockUser UseDB;
    EXPECT_CALL(UseDB, open).Times(::testing::AtLeast(1));
    EXPECT_CALL(UseDB, close).WillOnce(::testing::Return(open));
    EXPECT_CALL(UseDB, execQuery).Times(::testing::AtLeast(1));

    CreateThatUsesDB(&UseDB);
}

//---------------------------------------------
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

