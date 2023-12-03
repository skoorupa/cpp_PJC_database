#include "column.hpp"

namespace db {
    Column::Column(const std::string &name, DBType type) : name(name), type(type) {}

    const std::string &Column::getName() const {return name;}
    DBType Column::getType() const {return type;}
}