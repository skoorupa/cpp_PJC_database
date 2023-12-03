#include "table.hpp"
#include <fmt/core.h>

namespace db {
    Table::Table(const std::string &name) : name(name), columns(std::map<std::string, Column>()) {
        curr_row_id = 1;
    }

    auto Table::add_column(std::string name, DBType type) -> void {
        columns.insert(std::pair<std::string, Column>(name, Column(name, type)));
    }

    auto Table::add_row(std::vector<std::string> values) -> void {
        std::map<std::string, std::string> column_id_values = std::map<std::string, std::string>();
        // TODO: co jesli values jest wiecej/mniej niz columns
        auto i = 0;
        for (auto column : columns) {
            auto column_id = column.first;
            column_id_values[column_id] = values[i];
            i++;
        }

        rows.push_back(Row(curr_row_id, column_id_values));
        curr_row_id++;
    }

    auto Table::print() -> void {
        std::map<std::string, int> col_widths = std::map<std::string,int>();
        auto full_width = 0;
        for (auto column : columns) {
            auto column_id = column.first;
            int column_name_length = column_id.length();
            int max_length = column_name_length;
            for (Row row : rows) {
                std::string value = row.get_value(column_id);
                if (max_length < value.length()) max_length = value.length();
            }
            col_widths[column_id] = max_length;
            full_width += max_length+1;

            fmt::print("|");
            fmt::print("{}",column_id);
            for (int i = 0; i < max_length-column_name_length; ++i)
                fmt::print(" ");
        }
        fmt::println("|");
        for (int i = 0; i <= full_width; ++i)
            fmt::print("=");
        fmt::println("");

        for (Row row : rows) {
            for (auto column : columns) {
                auto column_id = column.first;
                auto value = row.get_value(column_id);

                fmt::print("|");
                fmt::print("{}", value);

                auto max_length = col_widths.at(column_id);
                for (int i = 0; i < max_length - value.length(); ++i)
                    fmt::print(" ");
            }

            fmt::println("|");
        }
    }
};
