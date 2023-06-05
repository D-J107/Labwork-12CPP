#include <gtest/gtest.h>
#include <lib/MyCoolDB.h>

TEST(Insert_tests, test1) {
    MyCoolDB my_first_db;

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE Customers(\n"
                    "ID   INT              NOT NULL,\n"
                    "NAME VARCHAR(5)     NOT NULL,\n"
                    "AGE  INT              ,\n"
                    "PRIMARY KEY (ID));"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                    "VALUES (1, 'Ivan', 555);"));

    ASSERT_FALSE(my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                    "VALUES (1, 'Ivan', 228);"));
    // Error! because primary key is unique

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                    "VALUES (5, 'Divan', 228);"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                    "VALUES (3, 'Divan', 340);"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Customers (ID, NAME)\n"
                    "VALUES (6, 'Dian');"));

    ASSERT_TRUE(my_first_db.send_request("SELECT AGE, NAME FROM Customers WHERE NAME = Divan;"));

    ASSERT_TRUE(my_first_db.send_request("SELECT * FROM Customers\n"
                    "WHERE NAME = 'Divan' AND ID > 4;"));
}
