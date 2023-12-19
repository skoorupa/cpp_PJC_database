#pragma once
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include "column.hpp"
#include "row.hpp"

namespace db {
    class Table {
        std::string name;
        std::vector<Column> columns;
        std::vector<Row> rows;
        int curr_row_id;  // wewnetrzne id dla programu

    public:
        Table(const std::string &name);

        const std::string &getName() const;
        auto rename(const std::string& new_name) -> void;
        auto add_column(std::string name, ColumnType type) -> void;
        auto get_column_iterator(const std::string& columnname);
        auto has_column(const std::string& columnname) -> bool;
        auto remove_column(const std::string& columnname) -> void;
        auto rename_column(const std::string& old_name, const std::string& new_name) -> void;
        auto add_row(std::vector<Value> values) -> void;
        auto print() -> void;
    };
};
