#include "../lib/MyCoolDB.h"
#include "examples.h"

void example1() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Customers(\n"
                             "  ID   INT              NOT NULL,\n"
                             "  NAME VARCHAR(5)     NOT NULL,\n"
                             "  AGE  INT              ,\n"
                             "  PRIMARY KEY (ID));");

    my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                             "VALUES (1, 'Ivan', 555);");

    my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                             "VALUES (1, 'Ivan', 228);");

    my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                             "VALUES (5, 'Divan', 228);");

    my_first_db.send_request("INSERT INTO Customers (ID, NAME, AGE)\n"
                             "VALUES (3, 'Divan', 340);");

    my_first_db.send_request("INSERT INTO Customers (ID, NAME)\n"
                             "VALUES (6, 'Dian');");

    my_first_db.send_request("SELECT AGE, NAME FROM Customers WHERE NAME = Divan;");

    my_first_db.send_request("SELECT * FROM Customers\n"
                             "WHERE NAME = 'Divan' AND ID > 4;");
}

void example2() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  shop_id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  PRIMARY KEY (name));");

    my_first_db.send_request("INSERT INTO Cats (shop_id, name) VALUES\n"
                             "(1, 'Murzik'),\n"
                             "(2, 'Kotoslav'),\n"
                             "(2, 'Jorik'),\n"
                             "(10, 'Vicont');");

    my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');");

    my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  PRIMARY KEY (id));");

    my_first_db.send_request("INSERT INTO Shops (id, shop_name) VALUES\n"
                             "(1, 'Four_paws'),\n"
                             "(2, 'Mr.Zoo'),\n"
                             "(3, 'Murzilla'),\n"
                             "(4, 'Cats&Dogs');");

    my_first_db.send_request("SELECT name, shop_name FROM Cats \n"
                             "INNER JOIN Shops ON shop_id = id;");
}

void example3() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  shop_id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  PRIMARY KEY (name));");

    my_first_db.send_request("INSERT INTO Cats (shop_id, name) VALUES\n"
                             "(1, 'Murzik'),\n"
                             "(2, 'Kotoslav'),\n"
                             "(2, 'Jorik'),\n"
                             "(10, 'Vicont');");

    my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');");

    my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  PRIMARY KEY (id));");

    my_first_db.send_request("INSERT INTO Shops (id, shop_name) VALUES\n"
                             "(1, 'Four_paws'),\n"
                             "(2, 'Mr.Zoo'),\n"
                             "(3, 'Murzilla'),\n"
                             "(4, 'Cats&Dogs');");

    my_first_db.send_request("SELECT name, shop_name FROM Cats \n"
                             "LEFT JOIN Shops ON shop_id = id;");
}

void example4() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  motherland VARCHAR(10),\n"
                             "  PRIMARY KEY (name));");

    my_first_db.send_request("INSERT INTO Cats (id, name, motherland) VALUES\n"
                             "(1, 'Murzik', 'RUS'),\n"
                             "(2, 'Kotoslav', 'UZB'),\n"
                             "(2, 'Jorik', 'JAP'),\n"
                             "(10, 'Vicont', 'USA');");

    my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');");

    my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  country VARCHAR(10)             ,\n"
                             "  PRIMARY KEY (id));");

    my_first_db.send_request("INSERT INTO Shops (id, shop_name, country) VALUES\n"
                             "(1, 'Four_paws', 'RUS'),\n"
                             "(2, 'Mr.Zoo', 'UZB'),\n"
                             "(3, 'Murzilla', 'JAP'),\n"
                             "(4, 'Cats&Dogs', 'USA');");

    my_first_db.send_request("SELECT name, shop_name FROM Cats \n"
                             "INNER JOIN Shops ON Cats.id = Shops.id AND Cats.motherland = Shops.country;");
}

void example5() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  motherland VARCHAR(10),\n"
                             "  PRIMARY KEY (name));");

    my_first_db.send_request("INSERT INTO Cats (id, name, motherland) VALUES\n"
                             "(1, 'Murzik', 'RUS'),\n"
                             "(2, 'Kotoslav', 'UZB'),\n"
                             "(2, 'Jorik', 'JAP'),\n"
                             "(10, 'Vicont', 'USA');");

    my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');");

    my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  country VARCHAR(10)             ,\n"
                             "  PRIMARY KEY (id));");

    my_first_db.send_request("INSERT INTO Shops (id, shop_name, country) VALUES\n"
                             "(1, 'Four_paws', 'RUS'),\n"
                             "(2, 'Mr.Zoo', 'UZB'),\n"
                             "(3, 'Murzilla', 'JAP'),\n"
                             "(4, 'Cats&Dogs', 'USA');");

    my_first_db.send_request("SELECT name, shop_name, motherland FROM Cats \n"
                             "RIGHT JOIN Shops ON Cats.id = Shops.id AND Cats.motherland = Shops.country;");
}

void example6() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  motherland VARCHAR(10),\n"
                             "  PRIMARY KEY (name));");

    my_first_db.send_request("INSERT INTO Cats (id, name, motherland) VALUES\n"
                             "(1, 'Murzik', 'RUS'),\n"
                             "(2, 'Kotoslav', 'UZB'),\n"
                             "(2, 'Jorik', 'JAP'),\n"
                             "(10, 'Vicont', 'USA');");

    my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');");

    my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  country VARCHAR(10)             ,\n"
                             "  PRIMARY KEY (id));");

    my_first_db.send_request("INSERT INTO Shops (id, shop_name, country) VALUES\n"
                             "(1, 'Four_paws', 'RUS'),\n"
                             "(2, 'Mr.Zoo', 'UZB'),\n"
                             "(3, 'Murzilla', 'JAP'),\n"
                             "(4, 'Cats&Dogs', 'USA');");

    my_first_db.send_request("SELECT name, shop_name, motherland FROM Cats \n"
                             "INNER JOIN Shops ON Cats.id = Shops.id WHERE Cats.motherland = Shops.country;");
}

void example7() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE Cats(\n"
                             "  id   INT                ,\n"
                             "  name VARCHAR(15)     NOT NULL,\n"
                             "  motherland VARCHAR(10),\n"
                             "  PRIMARY KEY (name));");

    my_first_db.send_request("INSERT INTO Cats (id, name, motherland) VALUES\n"
                             "(1, 'Murzik', 'RUS'),\n"
                             "(2, 'Kotoslav', 'UZB'),\n"
                             "(2, 'Jorik', 'JAP'),\n"
                             "(10, 'Vicont', 'USA');");

    my_first_db.send_request("INSERT INTO Cats (name) VALUES\n"
                             "('Zuza');");

    my_first_db.send_request("CREATE TABLE Shops(\n"
                             "  id   INT              NOT NULL,\n"
                             "  shop_name VARCHAR(10)     NOT NULL,\n"
                             "  country VARCHAR(10)             ,\n"
                             "  price INT                       ,\n"
                             "  PRIMARY KEY (id));");

    my_first_db.send_request("INSERT INTO Shops (id, shop_name, country, price) VALUES\n"
                             "(1, 'Four_paws', 'RUS', 400),\n"
                             "(2, 'Mr.Zoo', 'UZB', 500),\n"
                             "(3, 'Murzilla', 'JAP', 600),\n"
                             "(4, 'Cats&Dogs', 'USA', 700);");

    my_first_db.send_request("SELECT * FROM Shops;");

    my_first_db.send_request("UPDATE Shops SET price = 300, shop_name = 'Dogs&Cats' WHERE country = 'USA';");

    my_first_db.send_request("SELECT * FROM Shops;");
}

void example8() {
    MyCoolDB my_first_db;

    my_first_db.send_request("CREATE TABLE author(\n"
                             "    id INT PRIMARY KEY,\n"
                             "    name VARCHAR(15) NOT NULL,\n"
                             "    age INT\n"
                             "    );");

    my_first_db.send_request("CREATE TABLE books(\n"
                             "    id INT PRIMARY KEY,\n"
                             "    title VARCHAR(20) NOT NULL,\n"
                             "    price INT,\n"
                             "    author_id INT,\n"
                             "    FOREIGN KEY (author_id) REFERENCES author(id)\n"
                             ");");

    my_first_db.send_request("INSERT INTO author (id, name, age)\n"
                             "    VALUES (1, 'Jack_London', 40);");

    my_first_db.send_request("INSERT INTO author (id, name, age)\n"
                             "    VALUES (2, 'Lev_Tolstoy', 82);");

    my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                             "    VALUES (1, 'White_Fang', 299, 1);");

    my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                             "    VALUES (2, 'War_and_Peace', 499, 2);");

    my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                             "    VALUES (3, 'Call_of_the_Wild', 799, 1);");

    my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                             "    VALUES (4, 'Twenty_Chairs', 799, 4);");
    // Error! 4 - false field, because no author with id == 4

    my_first_db.send_request("INSERT INTO books (id, title, price, author_id)\n"
                             "    VALUES (4, 'Twenty_Chairs', 799, NULL);");
    // Error! foreign key cant reference to null

    my_first_db.send_request("DELETE FROM author WHERE name = 'Lev_Tolstoy';");
    // Error! because REFERENCE will be pointing to NULL, you should delete REFERENCE firstly

    my_first_db.send_request("DELETE FROM books WHERE author_id = 2;");
    // Success!

    my_first_db.send_request("DELETE FROM author WHERE name = 'Lev_Tolstoy';");
    // Now it is success request because no reference to books from Lev Tolstoy

    my_first_db.send_request("SELECT * FROM books;");
}