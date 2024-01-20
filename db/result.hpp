#pragma once

#include <vector>
#include "table.hpp"
#include "logic.hpp"

namespace db {
    ////////////////////////////////////////
    // SORTER

    enum class SortingMethod {
        Ascending, Descending
    };

    class Sorter {
        Column column;
        SortingMethod sortingMethod;
    public:
        Sorter(const Column &column, SortingMethod sortingMethod);

        const Column &getColumn() const;
        SortingMethod getSortingMethod() const;
        static auto toSortingMethod(const std::string&) -> SortingMethod;
    };

    ////////////////////////////////////////
    // RESULT

    class Result {
        std::vector<Table> tables;
        std::vector<Column> columns;
        std::vector<BinaryExpression> wheres;
        std::vector<Sorter> sorters;

    public:
        Result();

        const std::vector<Table> &getTables() const;
        const std::vector<Column> &getColumns() const;
        const std::vector<BinaryExpression> &getWheres() const;
        auto add_table(const Table& table) -> void;
        auto add_column(const Column& column) -> void;
        auto add_where(const db::BinaryExpression& binaryExpression) -> void;
        auto add_sorter(const Sorter& sorter) -> void;
        auto are_columns_blank() -> bool;
        auto are_wheres_blank() -> bool;
        auto update_value(const Value& value, const Row& row) -> Value;
        auto update_where(db::BinaryExpression binaryExpression, const Row& row) -> BinaryExpression;
        auto test_row(const Row& row) -> bool;
        auto getRows() -> std::vector<Row>;
        auto remove_rows(db::Table* table) -> void;
        auto update_rows(db::Table* table, const std::string& column_name, Value value) -> void;
        auto clear_tables() -> void;
        auto clear_columns() -> void;
        auto print() -> void;
        auto info() -> void;
    };

}