#include "table.hpp"
#include <fmt/core.h>

namespace db {
    /////////////////////////////////////
    // COLUMN CLASS

    Column::Column(int id, const std::string &name, DBType type) : id(id), name(name), type(type) {}

    int Column::getId() const {return id;}
    const std::string &Column::getName() const {return name;}
    DBType Column::getType() const {return type;}

    /////////////////////////////////////
    // ROW CLASS

    Row::Row(int id, const std::map<int, std::string> &values) : id(id), values(values) {}
    auto Row::get_value(int column_id) -> std::string {
        return values.at(column_id);
    }

    /////////////////////////////////////
    // TABLE CLASS

    Table::Table(const std::string &name) : name(name), columns(std::deque<Column>()) {
        curr_column_id = 1;
        curr_row_id = 1;
    }

    auto Table::add_column(std::string name, DBType type) -> void {
        columns.push_back(Column(curr_column_id, name, type));
        curr_column_id++;
    }

    auto Table::add_row(std::vector<std::string> values) -> void {
        std::map<int, std::string> column_id_values = std::map<int, std::string>();
        // TODO: co jesli values jest wiecej/mniej niz columns
        for (int i = 0; i < columns.size(); ++i) {
            auto column_id = columns.at(i).getId();
            column_id_values[column_id] = values[i];
        }

        rows.push_back(Row(curr_row_id, column_id_values));
        curr_row_id++;
    }
};
