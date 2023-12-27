#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include "column.hpp"

namespace db {
    class Row {
        int id;
        std::unordered_map<std::string, db::Value> values; // https://www.geeksforgeeks.org/map-vs-unordered_map-c/

    public:
        Row(int id, const std::unordered_map<std::string, db::Value> &values);
        auto get_value(const std::string& column_id) const -> db::Value;
        auto get_value_as_string(const std::string& column_id) const -> std::string;
        auto set_value(const std::string& column_id, const db::Value& value) -> void;
        auto remove_value(const std::string& column_id) -> void;
        auto has_column(const std::string& column_id) const -> bool;
        auto rename_column(const std::string& old_name, const std::string& new_name) -> void;
    };
}