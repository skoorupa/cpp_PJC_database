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
        const std::vector<Row> &getRows() const;
        const std::vector<Column> &getColumns() const;
        auto rename(const std::string& new_name) -> void;
        auto add_column(std::string name, ColumnType type, bool nullable) -> void;
        auto get_column_iterator(const std::string& columnname);
        auto get_row_iterator(int id);
        auto get_column(std::string columnname) -> Column;
        auto has_column(const std::string& columnname) const -> bool;
        auto remove_column(const std::string& columnname) -> void;
        auto rename_column(const std::string& old_name, const std::string& new_name) -> void;
        auto add_row(std::vector<Value> values) -> void;
        auto remove_row(int id) -> void;
        auto update_row(int id, std::string columnname, const Value& value) -> void;
        auto print() -> void;
        auto info() -> void;
    };

    auto format_as(const Table &table) -> std::string;
};
