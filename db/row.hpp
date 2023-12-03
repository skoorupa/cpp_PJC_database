#include <map>
#include <string>

namespace db {
    class Row {
        int id;
        std::map<std::string, std::string> values; // int - column_id, string - value

    public:
        Row(int id, const std::map<std::string, std::string> &values);
        auto get_value(std::string column_id) -> std::string;
    };
}