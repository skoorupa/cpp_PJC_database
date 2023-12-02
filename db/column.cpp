#include <string>
#include "column.hpp"

namespace db {
    Column::Column(int id, const std::string &name, DBType type) : id(id), name(name), type(type) {}

    int Column::getId() const {return id;}
    const std::string &Column::getName() const {return name;}
    DBType Column::getType() const {return type;}
}