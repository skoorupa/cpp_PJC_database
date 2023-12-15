#include <map>
#include <string>
#include "column.hpp"

namespace db {
    class Row {
        int id;
        std::map<std::string, db::Value> values; // int - column_id, string - value

    public:
        Row(int id, const std::map<std::string, db::Value> &values);
        auto get_value(const std::string& column_id) -> db::Value;
        auto get_value_as_string(const std::string& column_id) -> std::string;
        auto add_value(const std::string& column_id, const db::Value& value) -> void;
        auto has_column(const std::string& column_id) -> bool;
    };
}