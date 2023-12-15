#include <map>
#include <string>

namespace db {
    class Row {
        int id;
        std::map<std::string, std::string> values; // int - column_id, string - value

    public:
        Row(int id, const std::map<std::string, std::string> &values);
        auto get_value(const std::string& column_id) -> std::string;
        auto add_value(const std::string& column_id, std::string value) -> void;
        auto has_column(const std::string& column_id) -> bool;
    };
}