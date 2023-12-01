#include "table.hpp"

namespace db {
    /////////////////////////////////////
    // COLUMN CLASS

    Column::Column(const std::string &name, DBType type) : name(name), type(type) {}

    /////////////////////////////////////
    // TABLE CLASS

    Table::Table(const std::string &name) : name(name), columns(std::deque<Column>()) {}
    auto Table::add_column(std::string name, DBType type) {
        columns.push_back(Column(name, type));
    }
};
