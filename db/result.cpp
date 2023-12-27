//
// Created by adams on 19.12.2023.
//
#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include "result.hpp"

namespace db {
    db::Result::Result()
    : tables(std::vector<Table>()), columns(std::vector<Column>()), wheres(std::vector<BinaryExpression>()) {}

    // TODO: maybe change to pointers? we will see
    const std::vector<Table> &Result::getTables() const {return tables;}
    const std::vector<Column> &Result::getColumns() const {return columns;}
    const std::vector<BinaryExpression> &Result::getWheres() const {return wheres;}

    auto Result::add_table(Table table) -> void {
        tables.push_back(table);
    }

    auto Result::add_column(Column column) -> void {
        columns.push_back(column);
    }

    auto Result::add_where(db::BinaryExpression binaryExpression) -> void {
        wheres.push_back(binaryExpression);
    }

    auto Result::is_blank() -> bool {
        return columns.empty();
    }

    auto Result::update_value(const Value& v, const Row& row) -> Value {
        if (v.getType() != ColumnType::Identifier) return v;
        auto value = v;
        auto column_need = value.getValue();
        if (!row.has_column(column_need))
            throw fmt::format("< result does not have column {}", column_need);
//        for (const db::Table& t : tables) {
//            if (t.has_column(column_need)) {
//                value.setValue()
//            }
//        }
        auto data = row.get_value(column_need);
        value.setValue(data.getValue());
        value.setType(data.getType());
        return value;
    }

    auto Result::update_where(db::BinaryExpression binaryExpression, const Row& row) -> BinaryExpression {
        binaryExpression.setLeft(update_value(binaryExpression.getLeft(), row));
        binaryExpression.setRight(update_value(binaryExpression.getRight(), row));

        return binaryExpression;
    }

    auto Result::test_row(const Row &row) -> bool {
        for (BinaryExpression where : wheres) {
            // parsing where
            auto updated_where = update_where(where, row);
            if (updated_where.getLeft().getType() != updated_where.getRight().getType())
                throw fmt::format("< cannot compare {} with {}", where.getLeft(), where.getRight());

            // compare values here
            if (!updated_where.evaluate()) return false;
        }
        return true;
    }

    auto Result::getRows() -> std::vector<Row> {
        auto rows = std::vector<Row>();
        for (const Table& t : tables)
            for (const Row r : t.getRows())
                if (test_row(r)) rows.push_back(r);
        return rows;
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
            for (Row &row: getRows()) {
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


        for (Row row: getRows()) {
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