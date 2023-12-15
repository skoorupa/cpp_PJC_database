#pragma once
#include <string>
#include <map>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class ColumnType {
        Integer, String
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
    public:
        Column(const std::string &name, ColumnType type);

        int getId() const;
        const std::string &getName() const;
        ColumnType getType() const;

        static auto toColumnType(const std::string& str) -> ColumnType;
    };
}