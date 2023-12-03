#include "row.hpp"

namespace db {
    Row::Row(int id, const std::map<std::string, std::string> &values) : id(id), values(values) {}
    auto Row::get_value(std::string column_id) -> std::string {
        return values.at(column_id);
    }
}