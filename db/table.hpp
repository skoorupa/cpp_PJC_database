#pragma once
#include <string>
#include <deque>
#include <map>
#include <vector>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class DBType {
        Integer, String
    };

    /////////////////////////////////////
    // COLUMN CLASS
    class Column {
        int id;
        std::string name;
        DBType type;
    public:
        Column(int id, const std::string &name, DBType type);

        int getId() const;
        const std::string &getName() const;
        DBType getType() const;
    };

    /////////////////////////////////////
    // ROW CLASS
    class Row {
        int id;
        std::map<int, std::string> values; // int - column_id, string - value

    public:
        Row(int id, const std::map<int, std::string> &values);
        auto get_value(int column_id) -> std::string;
    };

    /////////////////////////////////////
    // TABLE CLASS
    class Table {
        std::string name;
        std::deque<Column> columns;
        std::deque<Row> rows;
        int curr_column_id; // wewnetrzne id dla programu
        int curr_row_id;  // wewnetrzne id dla programu

    public:
        Table(const std::string &name);

        auto add_column(std::string name, DBType type) -> void;
        auto add_row(std::vector<std::string> values) -> void;
        auto print();
    };
};
