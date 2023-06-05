#include "MyCoolDB.h"

MyCoolDB::Column::Column(std::string name, std::string type, bool must_be_no_null)
        : name_(std::move(name)), type_(std::move(type)), must_be_no_null_(must_be_no_null)
{}
