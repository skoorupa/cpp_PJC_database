#include "row.hpp"

namespace db {
    Row::Row(int id, const std::map<std::string, std::string> &values) : id(id), values(values) {}
    auto Row::get_value(const std::string& column_id) -> std::string {
        return values.at(column_id);
    }

    auto Row::add_value(const std::string& column_id, std::string value) -> void {
        values[column_id] = value;
    }

    auto Row::has_column(const std::string &column_id) -> bool {
        return values.contains(column_id);
    }
}