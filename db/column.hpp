#pragma once
#include <string>
#include <map>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class ColumnType {
        Integer, String, Null
    };

    /////////////////////////////////////
    // COLUMN CLASS
    class Value {
        std::string value;
        ColumnType type;

    public:
        Value(const std::string &value, ColumnType type);

        const std::string &getValue() const;
        ColumnType getType() const;
    };

    /////////////////////////////////////
    // COLUMN CLASS
    class Column {
        std::string name;
        ColumnType type;
        std::string table;
    public:
        Column(const std::string &table, const std::string &name, ColumnType type);

        int getId() const;
        const std::string &getName() const;
        ColumnType getType() const;
        void setName(const std::string &name);
        void setType(ColumnType type);

        static auto toColumnType(const std::string& str) -> ColumnType;
    };

    auto format_as(ColumnType columnType) -> std::string;
}