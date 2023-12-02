#include <map>
#include <string>

namespace db {
    class Row {
        int id;
        std::map<int, std::string> values; // int - column_id, string - value

    public:
        Row(int id, const std::map<int, std::string> &values);
        auto get_value(int column_id) -> std::string;
    };
}