#include <gtest/gtest.h>
#include <lib/MyCoolDB.h>

TEST(Drop_test, test1) {
    MyCoolDB my_first_db;

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE author(\n"
                                         "    id INT PRIMARY KEY,\n"
                                         "    name VARCHAR(15) NOT NULL,\n"
                                         "    age INT\n"
                                         "    );"));

    ASSERT_TRUE(my_first_db.send_request("CREATE TABLE books(\n"
                                         "    id INT PRIMARY KEY,\n"
                                         "    title VARCHAR(20) NOT NULL,\n"
                                         "    price INT,\n"
                                         "    author_id INT,\n"
                                         "    FOREIGN KEY (author_id) REFERENCES author(id)\n"
                                         ");"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO author (id, name, age)\n"
                                         "    VALUES (1, 'Jack_London', 40);"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO author (id, name, age)\n"
                                         "    VALUES (2, 'Lev_Tolstoy', 82);"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                                         "    VALUES (1, 'White_Fang', 299, 1);"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                                         "    VALUES (2, 'War_and_Peace', 499, 2);"));

    ASSERT_TRUE(my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                                         "    VALUES (3, 'Call_of_the_Wild', 799, 1);"));

    ASSERT_FALSE(my_first_db.send_request("DROP TABLE author;"));
    // Error because Books References to Author

    ASSERT_TRUE(my_first_db.send_request("DROP TABLE books;"));

    ASSERT_TRUE(my_first_db.send_request("DROP TABLE author;"));
}
