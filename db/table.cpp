#include "table.hpp"
#include <fmt/core.h>
#include <fmt/format.h>

namespace db {
    Table::Table(const std::string &name) :
        name(name),
        columns(std::vector<Column>()),
        rows(std::vector<Row>()),
        curr_row_id(1) {}

    const std::string &Table::getName() const {
        return name;
    }

    auto Table::rename(const std::string& new_name) -> void {
        name = new_name;
    }

    auto Table::add_column(std::string columnname, ColumnType type) -> void {
        columns.push_back(Column(name, columnname, type));
        fmt::println("< added new column to {} - {}",name,columnname);
        for (Row& row : rows)
            row.set_value(columnname, Value("null", ColumnType::Null));
    }

    auto Table::get_column_iterator(const std::string& columnname) {
        return std::ranges::find_if(
                columns.begin(),
                columns.end(),
                [columnname](const Column& column)->bool{return column.getName() == columnname;}
        );
    }

    auto Table::has_column(const std::string& columnname) -> bool {
        return get_column_iterator(columnname) != columns.end();
    }

    auto Table::remove_column(const std::string& columnname) -> void {
        auto col = get_column_iterator(columnname);
        if (col == columns.end())
            throw fmt::format("< cannot find column {} in table {}",columnname,name);

        columns.erase(col);

        for (Row& row : rows)
            row.remove_value(columnname);
    }

    auto Table::rename_column(const std::string& old_name, const std::string& new_name) -> void {
        auto col = get_column_iterator(old_name);
        if (col == columns.end())
            throw fmt::format("< cannot find column {} in table {}",old_name,name);
        if (has_column(new_name))
            throw fmt::format("< column {} already exists in table {}",old_name,name);

        col->setName(new_name);
        for (Row& row : rows)
            row.rename_column(old_name, new_name);
        fmt::println("< renamed column {} to {} in table {}",old_name,new_name,name);
    }

    auto Table::add_row(std::vector<Value> values) -> void {
        std::unordered_map<std::string, db::Value> column_id_values = std::unordered_map<std::string, db::Value>();
        // TODO: co jesli values jest wiecej/mniej niz columns
        auto i = 0;
        for (const auto& column : columns) {
            auto column_id = column.getName();
            column_id_values.insert(std::pair<std::string, db::Value>(column_id,values[i]));
            i++;
        }

        rows.push_back(Row(curr_row_id, column_id_values));
        curr_row_id++;
        fmt::println("< added new row to {}",name);
    }

    auto Table::print() -> void {
        fmt::println("< Table {}:", name);
        std::unordered_map<std::string, int> col_widths = std::unordered_map<std::string,int>();
        auto full_width = 0;
        for (const auto& column : columns) {
            auto column_id = column.getName();
            auto column_name_length = column_id.length();
            auto max_length = column_name_length;
            for (Row& row : rows) {
                if (row.has_column(column_id)) {
                    std::string value = row.get_value_as_string(column_id);
                    if (max_length < value.length()) max_length = value.length();
                }
            }
            col_widths[column_id] = max_length;
            full_width += max_length+1;

            fmt::print("|");
            fmt::print("{}",column_id);
            for (int i = 0; i < max_length - column_name_length; ++i)
                fmt::print(" ");
        }
        fmt::println("|");
        for (int i = 0; i <= full_width; ++i)
            fmt::print("=");
        fmt::println("");

        for (Row row : rows) {
            for (const auto& column : columns) {
                fmt::print("|");
                auto column_id = column.getName();
                auto letters = 0;

                if (row.has_column(column_id)) {
                    auto value = row.get_value_as_string(column_id);
                    letters = value.length();
                    fmt::print("{}", value);
                }

                auto max_length = col_widths.at(column_id);
                for (int i = 0; i < max_length - letters; ++i)
                    fmt::print(" ");
            }

            fmt::println("|");
        }
    }

    auto format_as(const Table& table) -> std::string {
        return table.getName();
    }
};
