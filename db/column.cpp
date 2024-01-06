#include "column.hpp"

namespace db {
    auto columntype_map = std::map<std::string, ColumnType>{
            {"integer", ColumnType::Integer},
            {"int", ColumnType::Integer},
            {"Integer", ColumnType::Integer},
            {"string", ColumnType::String},
            {"String", ColumnType::String},
            {"null", ColumnType::Null},
            {"Null", ColumnType::Null},
    };

    /////////////////////////////////////
    // VALUE CLASS

    Value::Value(const std::string &value, ColumnType type) : value(type==ColumnType::Null?"null":value), type(type) {}
    const std::string &Value::getValue() const {return value;}
    void Value::setValue(const std::string &value) {Value::value = value;}
    ColumnType Value::getType() const {return type;}
    void Value::setType(ColumnType type) {Value::type = type;}

    auto format_as(Value value) -> std::string {
        return fmt::format("Value(type: {}, value: {})", value.getType(), value.getValue());
    }

    /////////////////////////////////////
    // COLUMN CLASS
    Column::Column(const std::string &table, const std::string &name, ColumnType type, bool nullable)
    : table(table), name(name), type(type), nullable(nullable || type == ColumnType::Null) {}

    const std::string &Column::getName() const {return name;}
    ColumnType Column::getType() const {return type;}
    void Column::setName(const std::string &name) {Column::name = name;}
    void Column::setType(ColumnType type) {Column::type = type;}
    bool Column::isNullable() const {return nullable;}
    void Column::setNullable(bool nullable) {Column::nullable = nullable;}

    auto toColumnType(const std::string& str) -> ColumnType {
        return columntype_map.at(str);
    }

    auto format_as(Column column) -> std::string {
        auto result = fmt::format("{}(type: {}", column.getName(), column.getType());
        if (column.isNullable())
            result += ", nullable";
        return result+")";
    }

    auto format_as(ColumnType columnType) -> std::string {
        for (auto& pair : columntype_map)
            if (pair.second == columnType)
                return pair.first;
        
        return "UNKNOWN";
    }
}