#pragma once

#include <vector>
#include "table.hpp"
#include "logic.hpp"

namespace db {

    class Result {
        std::vector<Table> tables;
        std::vector<Row> rows;
        std::vector<Column> columns;

    public:
        Result();

        const std::vector<Table> &getTables() const;
        const std::vector<Row> &getRows() const;
        const std::vector<Column> &getColumns() const;
        auto add_table(Table table) -> void;
        auto add_row(Row row) -> void;
        auto add_column(Column column) -> void;
        auto is_blank() -> bool;
        auto print() -> void;
        auto where(BinaryExpression binaryExpression) -> void;
    };

}