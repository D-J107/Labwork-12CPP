#include "MyCoolDB.h"

bool MyCoolDB::send_request(CSR request) {
    std::vector<std::string> vec = SplitString(request);
    if (vec.back().back() != ';') {
        std::cout << "Error! semicolon missed in last argument!\n";
        return false;
    }
    enum {
        WAITING_FOR_NEXT_DATA = 0,
        CREATING_TABLE = 1,
        WAITING_FOR_COLUMN_PARAMETERS = 2,
        INSERTING_INTO_TABLE = 3,
        SELECTING = 4,
        DROPING = 5,
        UPDATING = 6,
        DELETING = 7
    };

    int state = 0;
    int i = 0;
    std::string buffer;
    while (i < vec.size()) {
        if (state == WAITING_FOR_NEXT_DATA) {
            if (vec[i] == "CREATE") {
                state = CREATING_TABLE;
                i++;
            }
            else if (vec[i] == "INSERT") {
                state = INSERTING_INTO_TABLE;
                i++;
            }
            else if (vec[i] == "SELECT") {
                state = SELECTING;
                i++;
            }
            else if (vec[i] == "DROP") {
                state = DROPING;
                i++;
            }
            else if (vec[i] == "UPDATE") {
                state = UPDATING;
                i++;
            }
            else if (vec[i] == "DELETE") {
                state = DELETING;
                i++;
            }
            else {
                std::cout << "Error! unknown argument!\n";
                return false;
            }
        }
        if ((vec[i] == ");" || vec[i] == ";") && i == vec.size() - 1) {
            return true;
        }
        if (state == CREATING_TABLE) {
            if (vec[i] != "TABLE") {
                std::cout << "Error! unknown object " << vec[i]  << '\n';
                return false;
            }
            if (vec.size() <= 2) {
                std::cout << "Error! no name given for Table!\n";
                return false;
            }
            i++;
            std::string table_name = vec[i];
            if ((table_name.back() == ')' && table_name[table_name.size() - 2] == '(') || table_name.back() == ';') {
                std::cout << "Error! Table must have at least 1 column";
                return false;
            }
            if (table_name.back() == '(') {
                table_name.pop_back();
            }
            if (vec[i+1].size() == 1 && vec[i+1][0] == '('){
                i++;
            }
            CREATE_TABLE(table_name);
            buffer = table_name;
            state = WAITING_FOR_COLUMN_PARAMETERS;
            i++;
        }

        if (state == WAITING_FOR_COLUMN_PARAMETERS) {
            bool primary_key = false;
            bool not_null = false;
            bool end_of_this_column = false;
            bool end_of_columns_arguments = false;

            std::string column_name = vec[i];

            if (column_name == "PRIMARY") {
                std::string key_name;
                if (!get_true_key_name(key_name,i,vec,end_of_columns_arguments,end_of_this_column)) {
                    return false;
                }

                CSR current_table_name = buffer;
                if (!table_array_[current_table_name].SET_PRIMARY_KEY(key_name)) {
                    std::cout << "Error! No key named \"" << key_name << "\"!\n";
                    return false;
                }
                if (end_of_this_column) state = WAITING_FOR_COLUMN_PARAMETERS;
                if (end_of_columns_arguments) state = WAITING_FOR_NEXT_DATA;
                i++;
                continue;

            }

            if (column_name == "FOREIGN") {
                std::string key_name;
                if (!get_true_key_name(key_name,i,vec,end_of_columns_arguments,end_of_this_column)) {
                    return false;
                }
                CSR current_table = buffer;
                i++;
                if (vec[i] != "REFERENCES") {
                    std::cout << "Error! keyword \"REFERENCES\" missed after foreign key!\n";
                    return false;
                }
                i++;
                std::string other_table = vec[i];
                if (other_table.back() != ')' && other_table.back() != ';') {
                    std::cout << "Error! brackets missed after \"REFERENCES\" argument!\n";
                    return false;
                }
                if (other_table.back() == ')' || other_table.back() == ';') other_table.pop_back();
                if (other_table.back() == ')' || other_table.back() == ';') other_table.pop_back();
                std::string other_column;
                size_t a = other_table.find('(');
                if (a == std::string::npos) {
                    std::cout << "Error! opening bracket missed!\n";
                    return false;
                }
                other_column = other_table.substr(a + 1);
                other_table = other_table.substr(0,a);
                if (other_table == current_table) {
                    std::cout << "Error! table \"" << current_table << "\" cannot point to itself!\n";
                    return false;
                }

                if (!TABLE(current_table).SET_FOREIGN_KEY(key_name,TABLE(other_table),other_column)) {
                    std::cout << "Error! \"" << key_name << "\" don't contains in table \"" << current_table << "\"! \n";
                    return false;
                }
                TABLE(other_table).what_reference_to_me_[current_table] = &TABLE(current_table);

                if (end_of_this_column) state = WAITING_FOR_COLUMN_PARAMETERS;
                if (end_of_columns_arguments) state = WAITING_FOR_NEXT_DATA;
                i++;
                continue;
            }
            if (column_name[0] == '(') {
                column_name = column_name.substr(1);
            }
            i++;
            std::string type = vec[i];

            if (type.back() == ';' && type[type.size() - 2] == ')') {
                type.pop_back();
                type.pop_back();
                primary_key = true;
                not_null = true;
                end_of_this_column = true;
                end_of_columns_arguments = true;
            }
            else if (type.back() == ',') {
                type.pop_back();
                end_of_this_column = true;
            }

            while (!end_of_this_column && !end_of_columns_arguments) {
                i++;
                std::string next_arg = vec[i];
                if ((next_arg == ");") || next_arg == ")" && vec[i+1] == ";") {
                    end_of_columns_arguments = true;
                    break;
                }
                if (next_arg == ",") {
                    end_of_this_column = true;
                    break;
                } else if (next_arg != "PRIMARY" && next_arg != "KEY" && next_arg != "NOT" && next_arg != "NULL") {
                    std::cout << "Error! wrong argument!: " << next_arg << '\n';
                    return false;
                }

                if (next_arg == "PRIMARY") {
                    i++;
                    if (vec[i] != "KEY" && vec[i] != "KEY," && vec[i] != "KEY);") {
                        std::cout << "Error! modifier PRIMARY must have keyword \"KEY\"\n";
                        return false;
                    }
                    primary_key = true;
                    if (vec[i] == "KEY,") {
                        end_of_this_column = true;
                    }
                    else if (vec[i] == "KEY);") {
                        end_of_columns_arguments = true;
                    }
                } if (next_arg == "NOT") {
                    i++;
                    if (vec[i] != "NULL" && vec[i] != "NULL," && vec[i] != "NULL);") {
                        std::cout << "Error! modifier NOT must have keyword \"NULL\"\n";
                        return false;
                    }
                    not_null = true;
                    if (vec[i] == "NULL,") {
                        end_of_this_column = true;
                    }
                    else if (vec[i] == "NULL);") {
                        end_of_columns_arguments = true;
                    }
                }
            }

            if (!check_type(type)) {
                std::cout << "Error! unknown type: " << type << '\n';
                return false;
            }

            CSR name_of_table = buffer;
            table_array_[name_of_table].ADD_COLUMN(column_name,type,not_null);
            if (primary_key) {
                table_array_[name_of_table].SET_PRIMARY_KEY(column_name);
            }

            i++;
            if (end_of_this_column) {
                state = WAITING_FOR_COLUMN_PARAMETERS;
            }
            if (end_of_columns_arguments) {
                state = WAITING_FOR_NEXT_DATA;
            }
        }
        if (state == INSERTING_INTO_TABLE) {
            bool end_of_current_arguments = false;
            bool end_of_all_arguments = false;
            std::vector<std::string> columns_names_buffer;
            std::vector<std::string> columns_values_buffer;
            if (vec[i] != "INTO") {
                std::cout << "Error! keyword \"INTO\" must presence after \"INSERT\" keyword!\n";
                std::cout << vec[i] << '\n';
                return false;
            }
            i++;
            CSR table_name = vec[i];
            if (!table_array_.contains(table_name)) {
                std::cout << "Error! no table \"" << table_name << "\"!\n";
                return false;
            }
            size_t amount = table_array_[table_name].get_columns_count();
            i++;
            std::string next_value = vec[i];
            if (next_value.size() == 1 && next_value[0] == '(') {
                i++;
            }
            for (int j = 0; j < amount; j++) {
                next_value = vec[i];
                if (next_value == "VALUES") {
                    break;
                }
                if (next_value.back() == ';') {
                    std::cout << "Error! names for column inserting have ended, but no values given for insert!\n";
                    return false;
                }
                if (next_value[0] == '(' || next_value[0] == ',') next_value = next_value.substr(1);
                if (next_value.back() == ')' || next_value.back() == ',') next_value.pop_back();
                columns_names_buffer.push_back(next_value);
                i++;
            }
            next_value = vec[i];
            if (next_value != "VALUES") {
                std::cout << "Error! \"VALUES\" keyword must be placed after table rows names!\n";
                return false;
            }

            i++;
            while(true) {
                columns_values_buffer.clear();
                end_of_current_arguments = false;
                for (int j = 0; j < amount; j++) {
                    next_value = vec[i];
                    if (next_value.back() == ';') {
                        next_value.pop_back();
                        end_of_all_arguments = true;
                        if (next_value.back() != ')') {
                            std::cout << "Error! closing brackets missing in these place: \"" << next_value << "\"!\n";
                            return false;
                        }
                        if (next_value.back() == ';') {
                            std::cout << "Error! obvious semicolon there: \"" << next_value << "\"!\n";
                            return false;
                        }
                        if (next_value.back() == ',') {
                            std::cout << "Error! obvious comma sign there: \"" << next_value << "\"!\n";
                            return false;
                        }
                    }
                    for (int jj = 0; jj < 2; jj++) {
                        if (next_value[0] == '(' || next_value[0] == ',') next_value = next_value.substr(1);
                        if (next_value.back() == ')') end_of_current_arguments = true;
                        if (next_value.back() == ')' || next_value.back() == ',') next_value.pop_back();
                    }
                    columns_values_buffer.push_back(next_value);
                    i++;
                    if (end_of_current_arguments || end_of_all_arguments) {
                        break;
                    }
                }
                if (!TABLE(table_name).INSERT_VALUES_INTO(columns_values_buffer, columns_names_buffer)) {
                    return false;
                }
                if (end_of_all_arguments) {
                    break;
                }
            }
            state = WAITING_FOR_NEXT_DATA;
        }
        if (state == SELECTING) {
            bool reached_end = false;
            std::string table_name;
            std::vector<std::string> what_select;
            std::vector<std::string> left;
            std::vector<std::string> statement;
            std::vector<std::string> right;
            std::vector<std::string> bool_exp;
            std::vector<std::string> extra_left;
            std::vector<std::string> extra_statement;
            std::vector<std::string> extra_right;
            std::vector<std::string> extra_bool_exp;
            std::string next_value = vec[i];
            if (next_value == "*") {
                i++;
                if (vec[i] != "FROM") {
                    std::cout << "Error! keyword \"FROM\" missed after SELECT!\n";
                    return false;
                }
                i++;
                table_name = vec[i];
                if (table_name.back() == ';') {
                    table_name.pop_back();
                    reached_end = true;
                }
                if (TABLE(table_name).name_ == "EMPTY") {
                    return false;
                }
                i++;
                for (const auto& j : table_array_[table_name].columns_) {
                    what_select.push_back(j.first);
                }
            } else {
                while (vec[i] != "FROM") {
                    if (vec[i].back() == ';') {
                        std::cout << "Error! statement ended without \"FROM\" keyword!\n";
                        return false;
                    }
                    if (vec[i] == "WHERE") {
                        std::cout << "Error! missed keyword \"FROM\" after select!\n";
                        return false;
                    }
                    if (vec[i] == ",") continue;
                    if (vec[i].back() == ',') vec[i].pop_back();
                    if (vec[i][0] == ',') vec[i] = vec[i].substr(1);
                    what_select.push_back(vec[i]);
                    i++;
                }
                i++;
                table_name = vec[i];
                if (table_name.back() == ';') {
                    table_name.pop_back();
                    reached_end = true;
                }
                i++;
            }
            bool join = false;
            bool join_with_where = false;
            std::string type_of_join;
            std::string other_table_name;
            if (reached_end) {
                if (i < vec.size()) {
                    std::cout << "Error! statement ended, but some keyword exists!!\n";
                    return false;
                }
            } else {
                if (vec[i] == "WHERE") {
                    get_values_for_select(i,left,statement,right,bool_exp,vec,reached_end);
                } else if (vec[i] == "JOIN") {
                    join = true;
                    type_of_join = "INNER";
                } else if (vec[i] == "INNER" || vec[i] == "LEFT" || vec[i] == "RIGHT" || vec[i] == "OUTER") {
                    type_of_join = vec[i];
                    i++;
                    if (vec[i] != "JOIN") {
                        std::cout << "Error! missed \"JOIN\" keyword!\n";
                        return false;
                    }
                    join = true;
                } else {
                    std::cout << "Error! unknown argument!\n";
                    return false;
                }
                if (join) {
                    i++;
                    other_table_name = vec[i];
                    if (TABLE(other_table_name).name_ == "EMPTY") {
                        std::cout << "Error! no table called: \"" << other_table_name << "\"!\n";
                        return false;
                    }
                    i++;
                    if (vec[i] != "ON") {
                        std::cout << "Error! keyword \"ON\" missed after JOIN!\n";
                        return false;
                    }

                }
            }
            if (join) {
                get_values_for_select(i,left,statement,right,bool_exp,vec,reached_end);
                if (!reached_end) {
                    get_values_for_select(i,extra_left,extra_statement,extra_right,
                                          extra_bool_exp,vec,reached_end);
                }
                if (!SELECT_WITH_JOIN(what_select, table_name, other_table_name,
                                      type_of_join, left, statement, right, bool_exp,
                                      extra_left,extra_statement,extra_right,extra_bool_exp)) {
                    return false;
                }
            } else if (!TABLE(table_name).SELECT(what_select, left, statement, right, bool_exp)) {
                return false;
            }
            state = WAITING_FOR_NEXT_DATA;
        }
        if (state == DROPING) {
            if (vec[i] != "TABLE") {
                std::cout << "Error! keyword \"TABLE\" missed after DROP keyword!\n";
                return false;
            }
            i++;
            std::string table_name = vec[i];
            if (table_name.back() == ';') {
                table_name.pop_back();
            }
            auto it = table_array_.find(table_name);
            if (it == table_array_.end()) {
                std::cout << "Error! no table with name: \"" << table_name << "\"!\n";
                return false;
            } else {
                if (!TABLE(table_name).what_reference_to_me_.empty()) {
                    std::cout << "Error! cant drop table \"" << table_name << "\" ";
                    std::cout << "because other tables have reference to this!\n";
                    return false;
                }
                CSR name_for_reference_key = TABLE(table_name).foreign_key_;
                if (!name_for_reference_key.empty()) {
                    Table& other_table = *TABLE(table_name).columns_.at(name_for_reference_key).reference_to_other_table_.first;
                    auto position = other_table.what_reference_to_me_.find(table_name);
                    other_table.what_reference_to_me_.erase(position);
                    TABLE(table_name).columns_.at(name_for_reference_key).reference_to_other_table_.first = nullptr;
                    TABLE(table_name).columns_.at(name_for_reference_key).reference_to_other_table_.second = "";
                }

                table_array_.erase(it);
                i++;
            }
        }
        if (state == UPDATING) {
            std::vector<std::string> what_select;
            std::vector<std::string> values;
            std::vector<std::string> left;
            std::vector<std::string> statement;
            std::vector<std::string> right;
            std::vector<std::string> bool_exp;
            bool reached_end = false;
            CSR table_name = vec[i];
            if (TABLE(table_name).name_ == "EMPTY") {
                std::cout << "Error! no table named \"" << table_name << "\"!\n";
                return false;
            }
            i++;
            if (vec[i] != "SET") {
                std::cout << "Error! keyword \"SET\" missed after UPDATE!\n";
                return false;
            }
            i++;
            while (true) {
                CSR column_name = vec[i];
                if (!TABLE(table_name).columns_.contains(column_name)) {
                    std::cout << "Error! no column named \"" << column_name << "\"!\n";
                    return false;
                }
                what_select.push_back(column_name);
                i++;
                if (vec[i] != "=") {
                    std::cout << "Error! equal sign missed to set values!\n";
                    return false;
                }
                i++;
                std::string value = vec[i];
                if (value.back() == ',') {
                    value.pop_back();
                }
                if (value.back() == ';') {
                    value.pop_back();
                    if (!Table::check_matching_type_and_value(
                            TABLE(table_name).columns_.at(column_name).type_, value)) {
                        return false;
                    }
                    if (value.back() == '\'') value.pop_back();
                    if (value[0] == '\'') value = value.substr(1);
                    values.push_back(value);
                    break;
                } else {
                    if (!Table::check_matching_type_and_value(
                            TABLE(table_name).columns_.at(column_name).type_, value)) {
                        return false;
                    }
                    if (value.back() == '\'') value.pop_back();
                    if (value[0] == '\'') value = value.substr(1);
                    values.push_back(value);
                    i++;
                    if (vec[i] == "WHERE") {
                        get_values_for_select(i,left,statement,right,bool_exp,vec,reached_end);
                        break;
                    }
                }
            }
            TABLE(table_name).UPDATE(what_select,values,left,statement,right,bool_exp);
            i++;
        }
        if (state == DELETING) {
            std::vector<std::string> left;
            std::vector<std::string> statement;
            std::vector<std::string> right;
            std::vector<std::string> bool_exp;
            bool reached_end;

            if (vec[i] != "FROM") {
                std::cout << "Error! keyword \"FROM\" missed after DELETE!\n";
                return false;
            }
            i++;
            std::string table_name = vec[i];
            i++;
            if (vec[i] == "WHERE") {
                get_values_for_select(i,left,statement,right,bool_exp,vec,reached_end);
            }
            if (!TABLE(table_name).DELETE(left,statement,right,bool_exp)) {
                return false;
            }
            state = WAITING_FOR_NEXT_DATA;
        }
    }
    return true;
}


