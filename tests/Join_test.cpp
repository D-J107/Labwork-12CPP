#include <gtest/gtest.h>
#include <lib/MyCoolDB.h>

TEST(Join_tests, test1) {
    MyCoolDB my_first_db;

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  shop_id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  PRIMARY KEY (name));"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Cats (shop_id, name) VALUES\n"
                             "(1, 'Murzik'),\n"
                             "(2, 'Kotoslav'),\n"
                             "(2, 'Jorik'),\n"
                             "(10, 'Vicont');"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');"));

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  PRIMARY KEY (id));"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Shops (id, shop_name) VALUES\n"
                             "(1, 'Four_paws'),\n"
                             "(2, 'Mr.Zoo'),\n"
                             "(3, 'Murzilla'),\n"
                             "(4, 'Cats&Dogs');"));

    ASSERT_TRUE(my_first_db.send_request("SELECT name, shop_name FROM Cats \n"
                             "INNER JOIN Shops ON shop_id = id;"));

    ASSERT_TRUE(my_first_db.send_request("SELECT name, shop_name FROM Cats \n"
                                         "LEFT JOIN Shops ON shop_id = id;"));
}

TEST(Join_tests, test2) {
    MyCoolDB my_first_db;

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  motherland VARCHAR(10),\n"
                             "  PRIMARY KEY (name));"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Cats (id, name, motherland) VALUES\n"
                             "(1, 'Murzik', 'RUS'),\n"
                             "(2, 'Kotoslav', 'UZB'),\n"
                             "(2, 'Jorik', 'JAP'),\n"
                             "(10, 'Vicont', 'USA');"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');"));

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  country VARCHAR(10)             ,\n"
                             "  PRIMARY KEY (id));"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Shops (id, shop_name, country) VALUES\n"
                             "(1, 'Four_paws', 'RUS'),\n"
                             "(2, 'Mr.Zoo', 'UZB'),\n"
                             "(3, 'Murzilla', 'JAP'),\n"
                             "(4, 'Cats&Dogs', 'USA');"));

    ASSERT_TRUE(my_first_db.send_request("SELECT name, shop_name FROM Cats \n"
                             "INNER JOIN Shops ON Cats.id = Shops.id AND Cats.motherland = Shops.country;"));

    ASSERT_TRUE(my_first_db.send_request("SELECT name, shop_name, motherland FROM Cats \n"
                             "RIGHT JOIN Shops ON Cats.id = Shops.id AND Cats.motherland = Shops.country;"));
}

