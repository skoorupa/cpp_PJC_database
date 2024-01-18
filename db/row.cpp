#include "row.hpp"

namespace db {
    Row::Row(int id, const std::unordered_map<Column, db::Value> &values) : id(id), values(values) {}

    int Row::getId() const {return id;}
    const std::unordered_map<Column, db::Value> &Row::getValuesMap() const {return values;};

    auto Row::get_value(const Column& column) const -> db::Value {
        return values.at(column);
    }

    auto Row::get_value_as_string(const Column& column) const -> std::string {
        return values.at(column).getValue();
    }

    auto Row::add_value(const Column& column, const db::Value& value) -> void {
        values.insert(std::pair<Column, db::Value>(column, value));
    }

    auto Row::update_value(const Column& column, const db::Value& value) -> void {
        values.at(column) = value;
    }

    auto Row::remove_value(const Column &column) -> void {
        values.erase(column);
    }

    auto Row::has_column(const Column &column) const -> bool {
        return values.contains(column);
    }

    auto Row::has_column_from_table(const std::string &column_name, const std::string &table_name) const -> bool {
        auto track = Column(table_name, column_name, ColumnType::Null, false);
        for (auto& [column, value] : values) {
            if (column == track)
                return true;
        }
        return false;
    }

    auto Row::get_column(const std::string &column_name, const std::string &table_name) -> Column {
        auto track = Column(table_name, column_name, ColumnType::Null, false);
        for (auto& [column, value] : values) {
            if (column == track)
                return column;
        }
        throw fmt::format("< cannot get column {} in table {}",column_name, table_name);
    }

    auto Row::rename_column(const Column& old_column, const Column& new_column) -> void {
        auto node = values.extract(old_column);
        node.key() = old_column;
        values.insert(std::pair<Column, Value>(new_column, node.mapped()));
    }

    auto Row::operator+(const Row& other) -> Row {
        // TODO: copy
        for (const auto& [columnname, value] : other.getValuesMap())
            add_value(columnname, value);
        return *this;
    }
}