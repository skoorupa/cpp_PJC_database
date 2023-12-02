#include "table.hpp"

namespace db {
    /////////////////////////////////////
    // COLUMN CLASS

    Column::Column(const std::string &name, DBType type, int id) : name(name), type(type), id(id) {}

    /////////////////////////////////////
    // TABLE CLASS

    Table::Table(const std::string &name) : name(name), columns(std::deque<Column>()) {
        curr_column_id = 1;
    }

    auto Table::add_column(std::string name, DBType type) {
        columns.push_back(Column(name, type, curr_column_id));
    }
};
