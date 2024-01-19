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
        bool nullable;
    public:
        Column(const std::string &table, const std::string &name, ColumnType type, bool nullable);

        const std::string &getName() const;
        ColumnType getType() const;
        bool isNullable() const;
        void setName(const std::string &name);
        void setType(ColumnType type);
        void setNullable(bool nullable);
        const std::string &getTable() const;
        auto saver() -> std::string;

        bool operator==(const Column& other) const;
    };

    auto toColumnType(const std::string& str) -> ColumnType;
    auto format_as(Column column) -> std::string;
    auto format_as(ColumnType columnType) -> std::string;
}

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
// https://en.cppreference.com/w/cpp/utility/hash
// potrzebne w Row
template <>
struct std::hash<db::Column>
{
    std::size_t operator()(const db::Column& c) const
    {
        using std::size_t;
        using std::hash;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        std::size_t h1 = std::hash<std::string>{}(c.getTable());
        std::size_t h2 = std::hash<std::string>{}(c.getName());
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};
