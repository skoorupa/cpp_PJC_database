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
    : tables(std::vector<Table>()), columns(std::vector<Column>()), wheres(std::vector<BinaryExpression>()), blank(true) {}

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

    auto Result::are_columns_blank() -> bool {
        return columns.empty();
    }

    auto Result::are_wheres_blank() -> bool {
        return wheres.empty();
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
        auto result = std::vector<Row>();

        if (tables.empty()) return result;

        auto rows = tables[0].getRows();

        for (int i = 1; i < tables.size(); ++i) {
            auto temprows = std::vector<Row>();
            for (const auto& tablerow : tables[i].getRows()) {
                for (auto& existingrow : rows) {
                    auto newrow = existingrow;
                    newrow = newrow + tablerow;
                    temprows.push_back(newrow);
                }
            }
            rows = temprows;
        }

        for (const Row& r : rows)
            if (test_row(r)) result.push_back(r);

        return result;
    }

    auto Result::remove_rows(db::Table* table) -> void {
        // TODO: implement multiple tables
        auto rows = getRows();
        for (Row r : rows) {
            auto id = r.getId();
            table->remove_row(id);
        }
    }

    auto Result::update_rows(db::Table *table, std::string column_name, Value value) -> void {
        auto rows = getRows();
        for (Row r : rows) {
            auto id = r.getId();
            table->update_row(id, column_name, value);
        }
    }

    auto Result::print() -> void {
        fmt::println("< Result");
        fmt::println("< Tables: {}", tables);
        std::unordered_map<std::string, int> col_widths = std::unordered_map<std::string, int>();
        auto full_width = 0;
        auto rows = getRows();
        auto print_columns = columns;
        if (print_columns.empty())
            for (Table& t : tables)
                for (const Column& c : t.getColumns())
                    print_columns.push_back(c);
        for (const auto &column: print_columns) {
            auto column_id = column.getName();
            auto column_name_length = column_id.length();
            auto max_length = column_name_length;
            for (Row& row: rows) {
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


        for (Row& row: rows) {
            for (const auto &column: print_columns) {
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

    auto Result::info() -> void {
        fmt::println("< result info");
        fmt::println("tables ({}): {}", tables.size(), tables);
        fmt::println("columns selected ({}): {}", columns.size(), columns);
        fmt::println("rows amount: {}", getRows().size());
    }
}