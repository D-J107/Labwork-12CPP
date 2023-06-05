#include "MyCoolDB.h"

bool MyCoolDB::SELECT_WITH_JOIN (CVSR what_select, CSR table1, CSR table2, CSR type,
                                 CVSR left, CVSR statement, VSR right, CVSR bool_exp,
                                 CVSR extra_left, CVSR extra_statement, VSR extra_right, CVSR extra_bool_exp) noexcept {
    for (CSR j: left) {
        size_t a = j.find('.');
        if (a == std::string::npos) {
            if (!TABLE(table1).columns_.contains(j)) {
                std::cout << "Error! no row named \"" << j << "\" in table " << table1 << "!\n";
                return false;
            }
        } else {
            if (!TABLE(j.substr(0,a)).columns_.contains(j.substr(a + 1))) {
                std::cout << "Error! no row named \"" << j << "\" in table " << table1 << "!\n";
                return false;
            }
        }
    }
    for (const std::string& j: right) {
        std::string copy = j;
        if (copy.back() == ';') {
            copy.pop_back();
        }
        size_t a = copy.find('.');
        if (a == std::string::npos) {
            if (!TABLE(table2).columns_.contains(copy)) {
                std::cout << "Error! no row named \"" << copy << "\" in table " << table2 << "!\n";
                return false;
            }
        } else {
            if (!TABLE(copy.substr(0,a)).columns_.contains(copy.substr(a + 1))) {
                std::cout << "Error! no row named \"" << j << "\" in table " << table1 << "!\n";
                return false;
            }
        }
    }
    size_t values_amount_in_first_table = TABLE(table1).columns_.begin()->second.values_.size();
    size_t values_amount_in_second_table = TABLE(table2).columns_.begin()->second.values_.size();
    for (const auto &j: what_select) {
        std::cout << j << ' ';
    }
    std::cout << '\n';

    auto less_than = [](CSR a, CSR b) -> bool {
        if (a.size() < b.size()) return true;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        return false;
    };

    std::vector<std::pair<int, int>> pairs;

    for (int i = 0; i < values_amount_in_first_table; i++) {
        for (int ii = 0; ii < values_amount_in_second_table; ii++) {
            int k = 0;
            std::vector<bool> vector;
            for (int j = 0; j < statement.size(); j++) {
                if (!extra_statement.empty() && !check_where_condition(extra_left,extra_statement,extra_right,
                                               extra_bool_exp, i,ii)) {
                    vector.emplace_back(false);
                    continue;
                }

                std::string maybe_left_value = left[j];
                std::string estimated_left_table_name = table1;
                parse_string_with_dot(maybe_left_value,estimated_left_table_name);
                CSR left_value = TABLE(estimated_left_table_name).columns_.at(maybe_left_value).values_[i];
                std::string expected_right_table_name = table2;
                bool b = false;
                if (statement[j] == "IN" || statement[j] == "!IN") {
                    b = value_contains_in_fields(k,right, expected_right_table_name,ii,left_value,statement[j]);
                }
                std::string maybe_right_value = right[k];
                if (maybe_right_value.back() == ';') {
                    maybe_right_value.pop_back();
                }
                parse_string_with_dot(maybe_right_value,expected_right_table_name);
                std::string right_value = TABLE(expected_right_table_name).columns_.at(maybe_right_value).values_[ii];
                if (statement[j] == "=") {
                    b = left_value == right_value;
                }
                else if (statement[j] == "<") {
                    b = less_than(left_value,right_value);
                }
                else if (statement[j] == ">") {
                    b = !less_than(left_value,right_value) && left_value != right_value;
                }
                else if (statement[j] == "<=") {
                    b = less_than(left_value,right_value) || left_value == right_value;
                }
                else if (statement[j] == ">=") {
                    b = !(less_than(left_value,right_value)) || left_value == right_value;
                }
                else if (statement[j] == "!=") {
                    b = left_value != right_value;
                }
                vector.push_back(b);
                k++;
            }
            bool b = vector[0];
            for (int jj = 0; jj < bool_exp.size(); jj++) {
                if (bool_exp[jj] == "AND") {
                    b = b && vector[jj + 1];
                } else if (bool_exp[jj] == "OR") {
                    b = b || vector[jj + 1];
                }
            }
            if (b) {
                pairs.emplace_back(i,ii);
            }
        }
    }

    bool left_have_pair[values_amount_in_first_table];
    bool right_have_pair[values_amount_in_second_table];
    print_inner_join(left_have_pair,right_have_pair,pairs,what_select,
                     TABLE(table1), TABLE(table2));
    if (type == "LEFT") {
        print_left_join(left_have_pair, values_amount_in_first_table, what_select,
                        TABLE(table1), TABLE(table2));
    } else if (type == "RIGHT") {
        print_right_join(right_have_pair, values_amount_in_second_table, what_select,
                         TABLE(table2),TABLE(table1));
    } else if (type == "OUTER") {
        print_left_join(left_have_pair, values_amount_in_first_table, what_select,
                        TABLE(table1), TABLE(table2));
        print_right_join(right_have_pair, values_amount_in_second_table, what_select,
                         TABLE(table2),TABLE(table1));
    }

    std::cout << '\n';
    return true;
}

bool MyCoolDB::value_contains_in_fields(int& k, VSR right, std::string& expected_right_table_name, int ii, CSR left_value, CSR key_word) noexcept {
    while (true) {
        bool c = false;
        std::string maybe_right_value = right[k];
        if (maybe_right_value.back() == ';') {
            maybe_right_value.pop_back();
            c = true;
        }
        parse_string_with_dot(maybe_right_value,expected_right_table_name);
        std::string right_value = TABLE(expected_right_table_name).columns_.at(maybe_right_value).values_[ii];
        if (left_value == right_value) {
            if (key_word == "IN") {
                return true;
            } else if (key_word == "!IN") {
                return false;
            }
        }
        if (c) {
            if (left_value == right_value) {
                if (key_word == "IN") {
                    return true;
                } else if (key_word == "!IN") {
                    return false;
                }
            } else {
                if (key_word == "IN") {
                    return false;
                } else if (key_word == "!IN") {
                    return true;
                }
            }
        }
        k++;
    }
}

void MyCoolDB::print_inner_join(bool* left, bool* right, const std::vector<std::pair<int, int>> &vec,
                                CVSR what_select, const MyCoolDB::Table &tab1,
                                const MyCoolDB::Table &tab2) noexcept {
    for (const auto& i: vec) {
        left[i.first] = true;
        right[i.second] = true;
        for (CSR row_name: what_select) {
            if (tab1.columns_.contains(row_name)) {
                std::cout << tab1.columns_.at(row_name).values_[i.first] << ' ';
            } else if (tab2.columns_.contains(row_name)) {
                std::cout << tab2.columns_.at(row_name).values_[i.second] << ' ';
            }
        }
        std::cout << '\n';
    }
}


void MyCoolDB::print_left_join(const bool* left, size_t left_size, CVSR what_select,
                               const Table& left_table, const Table& right_table) noexcept {
    for (int i = 0; i < left_size; i++) {
        if (!left[i]) {
            for (CSR row_name: what_select) {
                if (left_table.columns_.contains(row_name)) {
                    std::cout << left_table.columns_.at(row_name).values_[i] << ' ';
                } else if (right_table.columns_.contains(row_name)) {
                    std::cout << "NULL ";
                }
            }
            std::cout << '\n';
        }
    }
}

void MyCoolDB::print_right_join(const bool* right, size_t right_size, CVSR what_select,
                                const Table& right_table, const Table& left_table) noexcept {
    for (int i = 0; i < right_size; i++) {
        if (!right[i]) {
            for (CSR row_name: what_select) {
                if (left_table.columns_.contains(row_name)) {
                    std::cout << "NULL ";
                } else if (right_table.columns_.contains(row_name)) {
                    std::cout << right_table.columns_.at(row_name).values_[i] << ' ';
                }
            }
            std::cout << '\n';
        }
    }
}

void MyCoolDB::parse_string_with_dot(std::string& value, std::string& table_name) noexcept {
    size_t a = value.find('.');
    if (a == std::string::npos) return;
    table_name = value.substr(0,a);
    value = value.substr(a + 1);
}

bool MyCoolDB::check_where_condition(CVSR left, CVSR statement, VSR right, CVSR bool_exp, int i, int ii) noexcept {
    auto less_than = [](CSR a, CSR b) -> bool {
        if (a.size() < b.size()) return true;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        return false;
    };
    int k = 0;
    std::vector<bool> vector;
    for (int j = 0; j < statement.size(); j++) {
        std::string maybe_left_value = left[j];
        std::string estimated_left_table_name;
        parse_string_with_dot(maybe_left_value,estimated_left_table_name);
        CSR left_value = TABLE(estimated_left_table_name).columns_.at(maybe_left_value).values_[i];
        std::string expected_right_table_name;
        bool b = false;
        if (statement[j] == "IN" || statement[j] == "!IN") {
            b = value_contains_in_fields(k,right, expected_right_table_name,ii,left_value,statement[j]);
        }
        std::string maybe_right_value = right[k];
        if (maybe_right_value.back() == ';') {
            maybe_right_value.pop_back();
        }
        parse_string_with_dot(maybe_right_value,expected_right_table_name);
        std::string right_value = TABLE(expected_right_table_name).columns_.at(maybe_right_value).values_[ii];
        if (statement[j] == "=") {
            b = left_value == right_value;
        }
        else if (statement[j] == "<") {
            b = less_than(left_value,right_value);
        }
        else if (statement[j] == ">") {
            b = !less_than(left_value,right_value) && left_value != right_value;
        }
        else if (statement[j] == "<=") {
            b = less_than(left_value,right_value) || left_value == right_value;
        }
        else if (statement[j] == ">=") {
            b = !(less_than(left_value,right_value)) || left_value == right_value;
        }
        else if (statement[j] == "!=") {
            b = left_value != right_value;
        }
        vector.push_back(b);
        k++;
    }
    bool b = vector[0];
    for (int jj = 0; jj < bool_exp.size(); jj++) {
        if (bool_exp[jj] == "AND") {
            b = b && vector[jj + 1];
        } else if (bool_exp[jj] == "OR") {
            b = b || vector[jj + 1];
        }
    }
    return b;
}
