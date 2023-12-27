#include "column.hpp"

namespace db {
    auto columntype_map = std::map<std::string, ColumnType>{
            {"integer", ColumnType::Integer},
            {"int", ColumnType::Integer},
            {"Integer", ColumnType::Integer},
            {"string", ColumnType::String},
            {"String", ColumnType::String}
    };

    /////////////////////////////////////
    // VALUE CLASS

    Value::Value(const std::string &value, ColumnType type) : value(value), type(type) {}
    const std::string &Value::getValue() const {return value;}
    void Value::setValue(const std::string &value) {Value::value = value;}
    ColumnType Value::getType() const {return type;}
    void Value::setType(ColumnType type) {Value::type = type;}

    auto format_as(Value value) -> std::string {
        return fmt::format("Value(type: {}, value: {})", value.getType(), value.getValue());
    }

    /////////////////////////////////////
    // COLUMN CLASS
    Column::Column(const std::string &table, const std::string &name, ColumnType type) : table(table), name(name), type(type) {}

    const std::string &Column::getName() const {return name;}
    ColumnType Column::getType() const {return type;}
    void Column::setName(const std::string &name) {Column::name = name;}
    void Column::setType(ColumnType type) {Column::type = type;}

    auto toColumnType(const std::string& str) -> ColumnType {
        return columntype_map.at(str);
    }

    auto format_as(ColumnType columnType) -> std::string {
        for (auto& pair : columntype_map)
            if (pair.second == columnType)
                return pair.first;
        
        return "unknown";
    }
}