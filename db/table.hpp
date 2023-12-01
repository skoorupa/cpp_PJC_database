#include <string>
#include <deque>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class DBType {
        Integer, String
    };

    /////////////////////////////////////
    // COLUMN CLASS
    class Column {
        std::string name;
        DBType type;

    public:
        Column(const std::string &name, DBType type);
    };

    /////////////////////////////////////
    // TABLE CLASS
    class Table {
        std::string name;
        std::deque<Column> columns;

    public:
        Table(const std::string &name);

        auto add_column(std::string name, DBType type);
    };
};
