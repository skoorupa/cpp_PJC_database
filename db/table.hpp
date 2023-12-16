#pragma once
#include <string>
#include <deque>
#include <map>
#include <vector>
#include "column.hpp"
#include "row.hpp"

namespace db {
    class Table {
        std::string name;
        std::map<std::string, Column> columns;
        std::deque<Row> rows;
        int curr_row_id;  // wewnetrzne id dla programu

    public:
        Table(const std::string &name);

        auto add_column(std::string name, ColumnType type) -> void;
        auto remove_column(const std::string& name) -> void;
        auto add_row(std::vector<Value> values) -> void;
        auto print() -> void;
    };
};
