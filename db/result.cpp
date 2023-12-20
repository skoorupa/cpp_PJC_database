//
// Created by adams on 19.12.2023.
//
#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include "result.hpp"

namespace db {
    db::Result::Result() : tables(std::vector<Table>()), rows(std::vector<Row>()), columns(std::vector<Column>()) {}

    // TODO: maybe change to pointers? we will see
    const std::vector<Table> &Result::getTables() const {return tables;}
    const std::vector<Row> &Result::getRows() const {return rows;}
    const std::vector<Column> &Result::getColumns() const {return columns;}

    auto Result::add_table(Table table) -> void {
        tables.push_back(table);
    }

    auto Result::add_row(Row row) -> void {
        rows.push_back(row);
    }

    auto Result::add_column(Column column) -> void {
        columns.push_back(column);
    }

    auto Result::is_blank() -> bool {
        return columns.empty();
    }

    auto Result::print() -> void {
        fmt::println("< Result");
        fmt::println("< Tables: {}", tables);
        std::unordered_map<std::string, int> col_widths = std::unordered_map<std::string, int>();
        auto full_width = 0;
        for (const auto &column: columns) {
            auto column_id = column.getName();
            auto column_name_length = column_id.length();
            auto max_length = column_name_length;
            for (Row &row: rows) {
                if (row.has_column(column_id)) {
                    std::string value = row.get_value_as_string(column_id);
                    if (max_length < value.length()) max_length = value.length();
                }
            }
            col_widths[column_id] = max_length;
            full_width += max_length + 1;

            fmt::print("|");
            fmt::print("{}", column_id);
            for (int i = 0; i < max_length - column_name_length; ++i)
                fmt::print(" ");
        }
        fmt::println("|");
        for (int i = 0; i <= full_width; ++i)
            fmt::print("=");
        fmt::println("");

        for (const Table &table: tables) {
            for (Row row: table.getRows()) {
                for (const auto &column: columns) {
                    fmt::print("|");
                    auto column_id = column.getName();
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
    }
}