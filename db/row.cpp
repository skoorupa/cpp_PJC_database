#include "row.hpp"

namespace db {
    Row::Row(int id, const std::map<int, std::string> &values) : id(id), values(values) {}
    auto Row::get_value(int column_id) -> std::string {
        return values.at(column_id);
    }
}