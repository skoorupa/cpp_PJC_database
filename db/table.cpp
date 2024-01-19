#include "table.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>

namespace db {
    Table::Table(const std::string &name) :
        name(name),
        columns(std::vector<Column>()),
        rows(std::vector<Row>()),
        curr_row_id(1) {}

    const std::string &Table::getName() const {return name;}
    const std::vector<Row> &Table::getRows() const {return rows;}
    const std::vector<Column> &Table::getColumns() const {return columns;}

    auto Table::rename(const std::string& new_name) -> void {
        name = new_name;
    }

    auto Table::add_column(std::string columnname, ColumnType type, bool nullable) -> void {
        auto column = Column(name, columnname, type, nullable);
        columns.push_back(column);
        fmt::println("< added new column to {} - {}",name,columnname);
        for (Row& row : rows)
            row.add_value(column, Value("null", ColumnType::Null));
    }

    auto Table::get_column_iterator(const std::string& columnname) {
        return std::ranges::find_if(
            columns.begin(),
            columns.end(),
            [columnname](const Column& column)->bool{return column.getName() == columnname;}
        );
    }

    auto Table::get_row_iterator(int id) {
        return std::find_if(
            rows.begin(),
            rows.end(),
            [id](auto r){return r.getId() == id;}
        );
    }

    auto Table::get_column(std::string columnname) -> Column {
        if (!has_column(columnname))
            throw fmt::format("< cannot find column {} in table {}", columnname, name);

        return *get_column_iterator(columnname);
    }

    auto Table::has_column(const std::string& columnname) const -> bool {
        // nie mozna skorzystac z get_column_iterator, bo get_column_iterator nie jest const
        return std::ranges::find_if(
                columns.begin(),
                columns.end(),
                [columnname](const Column& column)->bool{return column.getName() == columnname;}
        ) != columns.end();
    }

    auto Table::remove_column(const std::string& columnname) -> void {
        auto col = get_column_iterator(columnname);
        if (col == columns.end())
            throw fmt::format("< cannot find column {} in table {}",columnname,name);

        columns.erase(col);

        for (Row& row : rows)
            row.remove_value(*col);
    }

    auto Table::rename_column(const std::string& old_name, const std::string& new_name) -> void {
        auto col = get_column_iterator(old_name);
        auto oldcol = *col;
        if (col == columns.end())
            throw fmt::format("< cannot find column {} in table {}",old_name,name);
        if (has_column(new_name))
            throw fmt::format("< column {} already exists in table {}",old_name,name);

        col->setName(new_name);
        for (Row& row : rows)
            row.rename_column(oldcol, *col);
        fmt::println("< renamed column {} to {} in table {}",old_name,new_name,name);
    }

    auto Table::add_row(std::vector<Value> values) -> void {
        if (values.size() != columns.size())
            throw fmt::format("< missing values to add new row: expected {} values, got {}", columns.size(), values.size());

        auto column_values = std::unordered_map<Column, db::Value>();
        auto i = 0;
        for (const auto& column : columns) {
            if (values[i].getType() == ColumnType::Null && !column.isNullable())
                throw fmt::format("< column {} is not nullable", column.getName());

            if (values[i].getType() != column.getType() && values[i].getType() != ColumnType::Null)
                throw fmt::format("< incorrect type provided for column {} - expected {}, got {}",
                                  column.getName(), column.getType(), values[i].getType());

            column_values.insert(std::pair<Column, db::Value>(column, values[i]));
            i++;
        }

        rows.push_back(Row(curr_row_id, column_values));
        curr_row_id++;
        fmt::println("< added new row to {}",name);
    }

    auto Table::remove_row(int id) -> void {
        auto row_iterator = get_row_iterator(id);

        if (row_iterator == rows.end())
            throw fmt::format("< cannot find this row in table {}", name);

        rows.erase(row_iterator);
        fmt::println("< removed row from table {}", name);
    }

    auto Table::update_row(int id, std::string columnname, const Value& value) -> void {
        auto row_iterator = get_row_iterator(id);
        auto column_iterator = get_column_iterator(columnname);

        if (row_iterator == rows.end())
            throw fmt::format("< cannot find this row in table {}", name);
        if (column_iterator == columns.end())
            throw fmt::format("< cannot find column {} in table {}",columnname,name);

        if (value.getType() == ColumnType::Null && !column_iterator->isNullable())
            throw fmt::format("< column {} is not nullable", columnname);
        if (column_iterator->getType() != value.getType() && value.getType() != ColumnType::Null)
            throw fmt::format("< wrong provided for column {}: expected {}, got {}", columnname, column_iterator->getType(), value.getType());

        row_iterator->update_value(*column_iterator, value);
        fmt::println("< updated value {} in row", columnname);
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
                if (row.has_column(column)) {
                    std::string value = row.get_value_as_string(column);
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

                if (row.has_column(column)) {
                    auto value = row.get_value_as_string(column);
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

    auto Table::info() -> void {
        fmt::println("< table info");
        fmt::println("table name: {}", name);
        fmt::println("columns ({}): {}", columns.size(), columns);
        fmt::println("row amount: {}", rows.size());
    }

    auto Table::saver() -> std::string {
        auto result = std::string("create_table("+name+") ");

        for (auto& column : columns)
            result += column.saver()+" ";

        for (auto& row : rows)
            result += row.saver(columns);

        return result;
    }

    auto format_as(const Table& table) -> std::string {
        return table.getName();
    }
};
