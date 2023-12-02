#include "table.hpp"
#include <fmt/core.h>

namespace db {
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

    auto Table::print() -> void {
        std::map<int, int> col_widths = std::map<int,int>();
        auto full_width = 0;
        for (Column column : columns) {
            int column_name_length = column.getName().length();
            int max_length = column_name_length;
            for (Row row : rows) {
                std::string value = row.get_value(column.getId());
                if (max_length < value.length()) max_length = value.length();
            }
            col_widths[column.getId()] = max_length;
            full_width += max_length+1;

            fmt::print("|");
            fmt::print("{}",column.getName());
            for (int i = 0; i < max_length-column_name_length; ++i)
                fmt::print(" ");
        }
        fmt::println("|");
        for (int i = 0; i <= full_width; ++i)
            fmt::print("=");
        fmt::println("");

        for (Row row : rows) {
            for (Column column : columns) {
                auto value = row.get_value(column.getId());

                fmt::print("|");
                fmt::print("{}", value);

                auto max_length = col_widths.at(column.getId());
                for (int i = 0; i < max_length - value.length(); ++i)
                    fmt::print(" ");
            }

            fmt::println("|");
        }
    }
};
