#ifndef LABWORK_12_COPY_MYCOOLDB_H
#define LABWORK_12_COPY_MYCOOLDB_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>


typedef const std::string& CSR;
typedef std::vector<std::string>& VSR;
typedef const std::vector<std::string>& CVSR;

class MyCoolDB {
private:
    struct Table;
    struct Column {
        std::string type_;
        std::string name_;
        bool must_be_no_null_;
        std::vector<std::string> values_;
        std::pair<Table*,std::string> reference_to_other_table_; // table + column name
        Column(std::string name, std::string type, bool must_be_no_null = true);
    };
    struct Table {
        std::string name_;
        std::string primary_key_;
        std::map<std::string, Column> columns_;
        std::string foreign_key_;
        std::map<std::string,Table*> what_reference_to_me_;
        Table();
        Table(std::string name, std::string primary_key = "");
        Table(const Table& other);
        [[nodiscard]] size_t get_columns_count() const noexcept;
        [[nodiscard]] bool vector_contains(CVSR vector, CSR value) const noexcept;
        static bool check_matching_type_and_value(CSR type, std::string& value) noexcept;
        static bool value_in_values(CSR left_value, std::string& right_value, int& jj, CSR statement) noexcept;
        bool check_statements(CVSR left, CVSR statement, VSR right, CVSR bool_exp, int i) noexcept;
        bool DELETE(CVSR left, CVSR statement, VSR right, CVSR bool_exp) noexcept;
        [[nodiscard]] bool SELECT(CVSR what_select, CVSR left, CVSR statement, VSR right, CVSR bool_exp) noexcept;
        bool SET_PRIMARY_KEY(CSR key) noexcept;
        bool SET_FOREIGN_KEY(CSR column_name, Table& other_table, CSR other_column_name) noexcept;
        void ADD_COLUMN(CSR name, CSR type, bool must_be_no_null = true);
        bool INSERT_VALUES_INTO(VSR& values, VSR& where);
        bool check_inserting_values(std::map<std::string,bool>& help, CVSR where, VSR values, int i) noexcept;
        void UPDATE(CVSR what_update, CVSR values_to_update,
                    CVSR left, CVSR statement, VSR right, CVSR bool_exp) noexcept;
    };
    std::map<std::string, Table> table_array_;
    void CREATE_TABLE(CSR name);
    [[nodiscard]] bool SELECT_WITH_JOIN(CVSR what_select, CSR table1, CSR table2, CSR type,
                                        CVSR left, CVSR statement, VSR right, CVSR bool_exp,
                                        CVSR extra_left, CVSR extra_statement, VSR extra_right, CVSR extra_bool_exp) noexcept;
    Table& TABLE(CSR table_name) noexcept;
    static void print_inner_join(bool* left, bool* right, const std::vector<std::pair<int,int>>& vec,
                                 CVSR what_select, const Table& tab1, const Table& tab2) noexcept;
    static void print_left_join(const bool* left, size_t left_size, CVSR what_select,
                                const Table& left_table, const Table& right_table) noexcept;
    static void print_right_join(const bool* right, size_t right_size, CVSR what_select,
                                 const Table& right_table, const Table& left_table) noexcept;
    static std::vector<std::string> SplitString(CSR str);
    static bool check_type(CSR type) noexcept;
    static void get_values_for_select(int& i, VSR left, VSR statement, VSR right,
                                      VSR bool_exp, CVSR vec, bool& reached_end) noexcept;
    static inline Table empty_table{"EMPTY", "NO-KEY"};
    static void parse_string_with_dot(std::string& value, std::string& table_name) noexcept;
    bool value_contains_in_fields(int& k, VSR right, std::string& expected_right_table_name,
                                  int ii, CSR left_value, CSR key_word) noexcept;
    bool check_where_condition(CVSR left, CVSR statement, VSR right, CVSR bool_exp, int i, int ii) noexcept;
    bool get_true_key_name(std::string& key_name, int& i, CVSR vec,
                         bool& end_of_columns_arguments, bool& end_of_this_column) noexcept;
public:
    bool send_request(CSR request);
    void save_DB_to_file(CSR file_name);
    void read_DB_from_file(CSR file_name);
};


#endif //LABWORK_12_COPY_MYCOOLDB_H
