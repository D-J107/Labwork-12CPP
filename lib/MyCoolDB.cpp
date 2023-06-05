#include "MyCoolDB.h"

void MyCoolDB::CREATE_TABLE(CSR name) {
    table_array_[name] = Table(name);
}

MyCoolDB::Table& MyCoolDB::TABLE(CSR table_name) noexcept {
    for (std::pair<const std::string, Table>& i : table_array_) {
        if (i.first == table_name) return i.second;
    }
    return empty_table;
}

std::vector<std::string> MyCoolDB::SplitString(CSR str) {
    std::istringstream iss(str);
    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}

bool MyCoolDB::check_type(CSR type) noexcept {
    if (type == "INT" || type == "BOOL" || type == "FLOAT" || type == "DOUBLE") {
        return true;
    }
    if (type.size() >= 10 && type.substr(0,8) == "VARCHAR(" && type[type.size() - 1] == ')') {
        std::string value = type.substr(8);
        value.pop_back();
        for (const char& i : value) {
            if (i < '0' || i > '9') return false;
        }
        if (value.size() == 1 && value[0] == '0') return false;
        return true;
    }
    return false;
}

void MyCoolDB::get_values_for_select
        (int& i, VSR left, VSR statement, VSR right,
         VSR bool_exp,  CVSR vec, bool& reached_end)  noexcept {
    while (true) {
        bool exclusive_where_case_for_join = false;
        i++;
        if (vec[i] == "WHERE") {
            reached_end = false;
            break;
        }
        left.push_back(vec[i]);
        i++;
        if (vec[i] == "NOT") {
            if (vec[i + 1] != "IN") {
                std::cout << "Error! keyword\"IN\" missed after NOT!\n";
                return;
            }
            statement.emplace_back("!IN");
            i++;
        } else {
            statement.push_back(vec[i]);
        }
        i++;
        while (true) {
            std::string value = vec[i];
            if (vec[i] == "WHERE") {
                reached_end = false;
                exclusive_where_case_for_join = true;
                break;
            }
            if (value == "AND" || value == "OR") {
                bool_exp.push_back(value);
                break;
            }
            if (value.back() == ';' || value.back() == ')') {
                if (value.back() == ';') {
                    reached_end = true;
                    i++;
                }
                while (value.back() == ';' || value.back() == ')' || value.back() == '\'' || value.back() == ',') {
                    value.pop_back();
                }
                while (value[0] == ',' || value[0] == '\'') {
                    value = value.substr(1);
                }
                value.push_back(';');
                right.push_back(value);
                break;
            }
            if (value.back() == '\'' || value.back() == ',') {
                value.pop_back();
            }
            while (value[0] == ',' || value[0] == '\'' || value[0] == '(') {
                value = value.substr(1);
            }
            right.push_back(value);
            i++;
        }
        if (reached_end || exclusive_where_case_for_join) {
            break;
        }
    }
}

bool MyCoolDB::get_true_key_name(std::string& key_name, int& i, CVSR vec, bool& end_of_columns_arguments, bool& end_of_this_column) noexcept {
    i++;
    key_name = vec[i];
    if (key_name.substr(0,3) != "KEY") {
        std::cout << "Error! keyword \"KEY\" invalid!\n";
        return false;
    }
    if (key_name == "KEY") {
        i++;
        key_name = vec[i];
    }
    if (key_name.back() == ';') {
        if (key_name[key_name.size() - 2] != ')' || key_name[key_name.size() - 3] != ')') {
            std::cout << "Error! keyword \"KEY(something);\" invalid!\n";
            return false;
        }
        while (key_name.back() == ';' || key_name.back() == ')') {
            key_name.pop_back();
        }
        end_of_columns_arguments = true;
        i++;
    }
    else if (key_name.back() == ',') {
        end_of_this_column = true;
        key_name.pop_back();
        if (key_name.back() == ')') key_name.pop_back();
        i++;
    }
    if (key_name.back() == ')') key_name.pop_back();
    if (key_name[0] == '(') {
        key_name = key_name.substr(1);
    }
    return true;
}

void MyCoolDB::save_DB_to_file(CSR file_name) {
    std::ofstream fout(file_name);
    if (!fout.is_open()) {
        std::cout << "Error! cant open file \"" << file_name << "\"!\n";
        return;
    }
    fout.write((char*)this, sizeof(MyCoolDB));
    fout.close();
}

void MyCoolDB::read_DB_from_file(CSR file_name) {
    std::ifstream fin(file_name);
    if (fin.is_open()) {
        std::cout << "Error! cant open file \"" << file_name << "\"!\n";
        return;
    }
    fin.read((char*)this, sizeof(MyCoolDB));
    fin.close();
}
