#include "table.hpp"
#include <fmt/core.h>

namespace db {
    Table::Table(const std::string &name) :
        name(name),
        columns(std::map<std::string, Column>()),
        curr_row_id(1) {}

    auto Table::add_column(std::string columnname, ColumnType type) -> void {
        columns.insert(std::pair<std::string, Column>(columnname, Column(columnname, type)));
        fmt::println("< added new column to {} - {}",name,columnname);
        for (Row& row : rows)
            row.set_value(columnname, Value("null", ColumnType::Null));
    }

    auto Table::remove_column(const std::string& name) -> void {
        columns.erase(name);
        for (Row& row : rows)
            row.remove_value(name);
    }

    auto Table::rename_column(const std::string& old_name, const std::string& new_name) -> void {
        // https://stackoverflow.com/questions/5743545/what-is-the-fastest-way-to-change-a-key-of-an-element-inside-stdmap
        // TODO: kolejnosc!!!
        auto node = columns.extract(old_name);
        node.key() = old_name;
        columns.insert(std::pair<std::string, Column>(new_name, node.mapped()));
        for (Row& row : rows)
            row.rename_column(old_name, new_name);
    }

    auto Table::add_row(std::vector<Value> values) -> void {
        std::map<std::string, db::Value> column_id_values = std::map<std::string, db::Value>();
        // TODO: co jesli values jest wiecej/mniej niz columns
        auto i = 0;
        for (auto column : columns) {
            auto column_id = column.first;
            column_id_values.insert(std::pair<std::string, db::Value>(column_id,values[i]));
            i++;
        }

        rows.push_back(Row(curr_row_id, column_id_values));
        curr_row_id++;
        fmt::println("< added new row to {}",name);
    }

    auto Table::print() -> void {
        fmt::println("< Table {}:", name);
        std::map<std::string, int> col_widths = std::map<std::string,int>();
        auto full_width = 0;
        for (auto column : columns) {
            auto column_id = column.first;
            auto column_name_length = column_id.length();
            auto max_length = column_name_length;
            for (Row row : rows) {
                if (row.has_column(column_id)) {
                    std::string value = row.get_value_as_string(column_id);
                    if (max_length < value.length()) max_length = value.length();
                }
            }
            col_widths[column_id] = max_length;
            full_width += max_length+1;

            fmt::print("|");
            fmt::print("{}",column_id);
            for (int i = 0; i < max_length - column_name_length; ++i)
                fmt::print(" ");
        }
        fmt::println("|");
        for (int i = 0; i <= full_width; ++i)
            fmt::print("=");
        fmt::println("");

        for (Row row : rows) {
            for (auto column : columns) {
                fmt::print("|");
                auto column_id = column.first;
                auto letters = 0;

                if (row.has_column(column_id)) {
                    auto value = row.get_value_as_string(column_id);
                    letters = value.length();
                    fmt::print("{}", value);
                }

                auto max_length = col_widths.at(column_id);
                for (int i = 0; i < max_length - letters; ++i)
                    fmt::print(" ");
            }

            fmt::println("|");
        }
    }
};
