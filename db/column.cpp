#include "column.hpp"

namespace db {
    Column::Column(const std::string &name, ColumnType type) : name(name), type(type) {}

    const std::string &Column::getName() const {return name;}
    ColumnType Column::getType() const {return type;}
}