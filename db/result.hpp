#pragma once

#include <vector>
#include "table.hpp"
#include "logic.hpp"

namespace db {

    class Result {
        std::vector<Table> tables;
        std::vector<Column> columns;
        std::vector<BinaryExpression> wheres;
        bool blank;

    public:
        Result();

        const std::vector<Table> &getTables() const;
        const std::vector<Column> &getColumns() const;
        const std::vector<BinaryExpression> &getWheres() const;
        auto add_table(Table table) -> void;
        auto add_column(Column column) -> void;
        auto add_where(db::BinaryExpression binaryExpression) -> void;
        auto is_blank() -> bool;
        static auto update_value(const Value& value, const Row& row) -> Value;
        auto update_where(db::BinaryExpression binaryExpression, const Row& row) -> BinaryExpression;
        auto test_row(const Row& row) -> bool;
        auto getRows() -> std::vector<Row>;
        auto print() -> void;
    };

}