#include "row.hpp"

namespace db {
    Row::Row(int id, const std::unordered_map<std::string, db::Value> &values) : id(id), values(values) {}
    auto Row::get_value(const std::string& column_id) -> db::Value {
        return values.at(column_id);
    }

    auto Row::get_value_as_string(const std::string& column_id) -> std::string {
        return values.at(column_id).getValue();
    }

    auto Row::set_value(const std::string& column_id, const db::Value& value) -> void {
        values.insert(std::pair<std::string, db::Value>(column_id, value));
    }

    auto Row::remove_value(const std::string &column_id) -> void {
        values.erase(column_id);
    }

    auto Row::has_column(const std::string &column_id) -> bool {
        return values.contains(column_id);
    }

    auto Row::rename_column(const std::string &old_name, const std::string &new_name) -> void {
        auto node = values.extract(old_name);
        node.key() = old_name;
        values.insert(std::pair<std::string, Value>(new_name, node.mapped()));
    }
}