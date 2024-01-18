#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "column.hpp"

namespace db {
    class Row {
        int id;
        std::unordered_map<Column, db::Value> values; // https://www.geeksforgeeks.org/map-vs-unordered_map-c/

    public:
        Row(int id, const std::unordered_map<Column, db::Value> &values);
        int getId() const;
        const std::unordered_map<Column, db::Value> &getValuesMap() const;

        auto get_value(const Column& column) const -> db::Value;
        auto get_value_as_string(const Column& column) const -> std::string;
        auto add_value(const Column& column, const db::Value& value) -> void;
        auto update_value(const Column& column, const db::Value& value) -> void;
        auto remove_value(const Column& column) -> void;
        auto has_column(const Column& column) const -> bool;
        auto has_column_from_table(const std::string& column_name, const std::string& table_name) const -> bool;
        auto get_column(const std::string& column_name, const std::string& table_name) -> Column;
        auto rename_column(const Column& old_column, const Column& new_column) -> void;

        auto operator+(const Row&) -> Row;
    };
}