#include "MyCoolDB.h"

MyCoolDB::Table::Table()
        : name_ ("NULL"), primary_key_ ("NULL")
{}

MyCoolDB::Table::Table(std::string name, std::string primary_key)
        : name_ (std::move(name)), primary_key_ (std::move(primary_key))
{}

MyCoolDB::Table::Table(const MyCoolDB::Table &other)
        : name_ (other.name_), primary_key_ (other.primary_key_), columns_(other.columns_)
{}

void MyCoolDB::Table::ADD_COLUMN(CSR name, CSR type, bool must_be_no_null) {
    columns_.insert(std::make_pair(name, Column(name,type,must_be_no_null)));
}

bool MyCoolDB::Table::SET_PRIMARY_KEY(CSR key) noexcept {
    if (columns_.contains(key)) {
        this->columns_.at(key).must_be_no_null_ = true;
        this->primary_key_ = key;
        return true;
    }
    return false;
}

bool MyCoolDB::Table::SET_FOREIGN_KEY(CSR column_name, Table& other_table, CSR other_column_name) noexcept {
    if (columns_.contains(column_name)) {
        this->columns_.at(column_name).reference_to_other_table_ = std::make_pair(&other_table,other_column_name);
        this->columns_.at(column_name).must_be_no_null_ = true;
        this->foreign_key_ = column_name;
        return true;
    }
    return false;
}

size_t MyCoolDB::Table::get_columns_count() const noexcept {
    return columns_.size();
}

bool MyCoolDB::Table::INSERT_VALUES_INTO(std::vector<std::string>& values, std::vector<std::string>& where) {
    std::map<std::string,bool> help;
    for (const auto& i : columns_) {
        help[i.first] = false;
    }
    for (int i = 0; i < where.size(); i++) {
        if (!check_inserting_values(help, where, values, i)) {
            return false;
        }
    }
    for (int i = 0; i < where.size(); i++) {
        columns_.at(where[i]).values_.push_back(values[i]);
    }

    for (auto& i : columns_) {
        if (!help[i.first]) {
            if (i.second.must_be_no_null_) {
                std::cout << "Error! missed argument for NOT NULL column: \"" << i.second.name_ << "\"\n";
                return false;
            }
            i.second.values_.emplace_back("NULL");
        }
    }
    return true;
}

bool MyCoolDB::Table::check_matching_type_and_value(CSR type, std::string& value) noexcept {
    if (value == "NULL") {
        return true;
    }
    if (type == "INT") {
        for (const char& i : value) {
            if (i < '0' || i > '9') return false;
        }
        return true;
    }
    if (type.substr(0,7) == "VARCHAR") {
        std::string int_string;
        for (int j = 8; j < type.size() - 1; j++) {
            int_string.push_back(type[j]);
        }
        int max_len = std::stoi(int_string);
        if (value[0] != '\'' || value.back() != '\'') {
            return false;
        } else {
            value.pop_back();
            value = value.substr(1);
        }
        if (value.size()  > max_len) {
            return false;
        }

        return true;
    }
    if (type == "BOOL") {
        if (value != "TRUE" && value != "FALSE") return false;
        return true;
    }
    if (type == "FLOAT" || type == "DOUBLE") {
        bool have_dot = false;
        for (const char& i : value) {
            if (!have_dot && i == '.') have_dot = true;
            if (have_dot && i == '.') {
                return false;
            }
        }
        if (!have_dot) return false;
        return true;
    }
    return false;
}

bool MyCoolDB::Table::SELECT
        (CVSR what_select, CVSR left, CVSR statement, VSR right, CVSR bool_exp) noexcept {
    for (CSR j : left) {
        if (!columns_.contains(j)) {
            std::cout << "Error! no row named \"" << j << "\"!\n";
            return false;
        }
    }
    size_t column_size = columns_.begin()->second.values_.size();
    for (const auto& j : what_select) {
        std::cout << j << ' ';
    }
    std::cout << '\n';

    for (int i = 0; i < column_size; i++) {
        if (check_statements(left,statement,right,bool_exp,i)) {
            for (const auto& j : what_select) {
                std::cout << columns_.at(j).values_[i] << ' ';
            }
            std::cout << '\n';
        }
    }
    std::cout << '\n';
    return true;
}

bool MyCoolDB::Table::value_in_values(CSR left_value, std::string& right_value, int& jj, CSR statement) noexcept {
    while (true) {
        if (left_value == right_value) {
            if (statement == "IN") {
                return true;
            } else if (statement == "!IN") {
                return false;
            }
        }
        if (right_value.back() == ';') {
            right_value.pop_back();
            if (left_value == right_value) {
                if (statement == "IN") {
                    return true;
                } else if (statement == "!IN") {
                    return false;
                }
            } else {
                if (statement == "IN") {
                    return false;
                } else if (statement == "!IN") {
                    return true;
                }
            }
        }
        jj++;
    }
}

void MyCoolDB::Table::UPDATE(CVSR what_update, CVSR values_to_update,
                             CVSR left, CVSR statement, VSR right, CVSR bool_exp) noexcept {
    size_t table_columns_size = this->columns_.begin()->second.values_.size();
    for (int i = 0; i < table_columns_size; i++) {
        if (check_statements(left,statement,right,bool_exp,i)) {
            for (int j = 0; j < what_update.size(); j++) {
                this->columns_.at(what_update[j]).values_[i] = values_to_update[j];
            }
        }
    }
}

bool MyCoolDB::Table::check_statements(CVSR left, CVSR statement, VSR right, CVSR bool_exp, int i) noexcept {
    auto less_than = [](CSR a, CSR b) -> bool {
        if (a.size() < b.size()) return true;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] < b[i]) return true;
            if (a[i] > b[i]) return false;
        }
        return false;
    };
    int k = 0;
    std::vector<bool> vector{};
    for (int j = 0; j < statement.size(); j++) {
        bool a = false;
        CSR left_value = columns_.at(left[j]).values_[i];
        std::string right_value = right[k];
        if (statement[j] == "IN" || statement[j] == "!IN") {
            a = value_in_values(left_value,right_value,k,statement[j]);
        }
        if (right_value.back() == ';') {
            right_value.pop_back();
        }
        if (statement[j] == "=") {
            a = left_value == right_value;
        }
        else if (statement[j] == "<") {
            a = less_than(left_value, right_value);
        }
        else if (statement[j] == ">") {
            a = !less_than(left_value, right_value) && left_value != right_value;
        }
        else if (statement[j] == "<=") {
            a = less_than(left_value, right_value) || left_value == right_value;
        }
        else if (statement[j] == ">=") {
            a = !less_than(left_value, right_value) || left_value == right_value;
        }
        else if (statement[j] == "!=") {
            a = left_value != right_value;
        }
        vector.push_back(a);
        k++;
    }
    bool a;
    if (vector.empty()) { // no condition case, like SELECT * FROM A;
        a = true;
    } else {
        a = vector[0];
    }
    for (int j = 0; j < bool_exp.size(); j++) {
        if (bool_exp[j] == "AND") {
            a = a && vector[j + 1];
        } else if (bool_exp[j] == "OR") {
            a = a || vector[j + 1];
        }
    }
    return a;
}

bool MyCoolDB::Table::vector_contains(CVSR vector, CSR value) const noexcept {
    for (CSR i : vector) {
        if (i == value) return true;
    }
    return false;
}

bool MyCoolDB::Table::DELETE(CVSR left, CVSR statement, VSR right, CVSR bool_exp) noexcept {
    size_t columns_values_size = columns_.begin()->second.values_.size();
    std::vector<int> indexes_to_delete;
    for (int i = 0; i < columns_values_size; i++) {
        if (!check_statements(left,statement,right,bool_exp,i)) {
            continue;
        }
        indexes_to_delete.push_back(i);
        if (!what_reference_to_me_.empty()) {
            for (const auto& other_tables : what_reference_to_me_) {
                const Table& other_table = *other_tables.second;
                CVSR other_values = other_table.columns_.at(other_table.foreign_key_).values_;
                CSR name_for_our_column =
                        other_table.columns_.at(other_table.foreign_key_).reference_to_other_table_.second;
                CVSR values = columns_.at(name_for_our_column).values_;
                CSR value = values[i];
                auto it = std::find(other_values.begin(), other_values.end(), value);
                if (it != other_values.end()) {
                    std::cout << "Error! values from table \"" << this->name_ << "\" ";
                    std::cout << "because other table \"" << other_table.name_ << "\" have reference ";
                    std::cout << "linked by this value: \"" << value << "\"!\n";
                    return false;
                }
            }
        }
    }
    for (int i : indexes_to_delete) {
        for (auto& j : columns_) {
            std::vector<std::string>& values = j.second.values_;
            values.erase(values.begin() + i);
        }
    }
    return true;
}

bool MyCoolDB::Table::check_inserting_values(std::map<std::string,bool>& help, CVSR where, VSR values, int i) noexcept {
    help[where[i]] = true;
    if (!columns_.contains(where[i])) {
        std::cout << "Error! no column named \"" << where[i] << "\"!\n";
        return false;
    }
    CSR type = columns_.at(where[i]).type_;
    if (!check_matching_type_and_value(type,values[i])) {
        std::cout << "Error! value \"" << values[i] << "\" have incorrect type!\n";
        return false;
    }
    if (where[i] == primary_key_) {
        CVSR values_in_primary_key = columns_.at(primary_key_).values_;
        auto it = std::find(
                std::begin(values_in_primary_key), std::end(values_in_primary_key), values[i]);
        if (it != values_in_primary_key.end()) {
            std::cout << "Error! value for primary key must be unique!\n";
            return false;
        }
    }
    if (where[i] == foreign_key_) {
        const Table* other_table = columns_.at(foreign_key_).reference_to_other_table_.first;
        const std::string& other_table_column = columns_.at(foreign_key_).reference_to_other_table_.second;
        if (values[i] == "NULL") {
            std::cout << "Error! FOREIGN KEY value cannot reference to empty object!\n";
            return false;
        }
        if (!vector_contains(other_table->columns_.at(other_table_column).values_,values[i])) {
            std::cout << "Error! value \"" << values[i] << "\" cant reference to non-exists value!\n";
            return false;
        }
    }
    return true;
}


