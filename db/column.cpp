#include "column.hpp"

namespace db {
    auto columntype_map = std::map<std::string, ColumnType>{
            {"integer", ColumnType::Integer},
            {"int", ColumnType::Integer},
            {"Integer", ColumnType::Integer},
            {"string", ColumnType::String},
            {"String", ColumnType::String}
    };

    Value::Value(const std::string &value, ColumnType type) : value(value), type(type) {}

    const std::string &Value::getValue() const {
        return value;
    }

    ColumnType Value::getType() const {
        return type;
    }

    Column::Column(const std::string &name, ColumnType type) : name(name), type(type) {}

    const std::string &Column::getName() const {return name;}
    ColumnType Column::getType() const {return type;}

    auto Column::toColumnType(const std::string& str) -> ColumnType {
        return columntype_map.at(str);
    }
}