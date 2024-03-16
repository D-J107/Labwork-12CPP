#include <gtest/gtest.h>
#include <lib/MyCoolDB.h>

TEST(Update_test, test1) {
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
                             "  price INT                       ,\n"
                             "  PRIMARY KEY (id));"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO Shops (id, shop_name, country, price) VALUES\n"
                             "(1, 'Four_paws', 'RUS', 400),\n"
                             "(2, 'Mr.Zoo', 'UZB', 500),\n"
                             "(3, 'Murzilla', 'JAP', 600),\n"
                             "(4, 'Cats&Dogs', 'USA', 700);"));

    ASSERT_TRUE(my_first_db.send_request("SELECT * FROM Shops;"));

    ASSERT_TRUE(my_first_db.send_request("UPDATE Shops SET price = 300, shop_name = 'Dogs&Cats' WHERE country = 'USA';"));

    ASSERT_TRUE(my_first_db.send_request("SELECT * FROM Shops;"));
}