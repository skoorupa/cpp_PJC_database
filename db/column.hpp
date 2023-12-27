#pragma once
#include <string>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class ColumnType {
        Integer, String, Null,
        Boolean, Identifier // for logic
    };

    /////////////////////////////////////
    // VALUE CLASS
    class Value {
        std::string value;
        ColumnType type;

    public:
        Value(const std::string &value, ColumnType type);

        virtual const std::string &getValue() const;
        void setValue(const std::string &value);
        ColumnType getType() const;
        void setType(ColumnType type);
    };

    auto format_as(Value value) -> std::string;

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
    };
    auto toColumnType(const std::string& str) -> ColumnType;
    auto format_as(ColumnType columnType) -> std::string;
}