#include <string>
#include <deque>
#include <map>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class DBType {
        Integer, String
    };

    /////////////////////////////////////
    // COLUMN CLASS
    class Column {
        int id;
        std::string name;
        DBType type;
    public:
        Column(int id, const std::string &name, DBType type);

        int getId() const;
        const std::string &getName() const;
        DBType getType() const;
    };

    /////////////////////////////////////
    // ROW CLASS
    class Row {
        int id;
        std::map<int, std::string> values; // int - column_id, string - value

    public:
        Row(int id, const std::map<int, std::string> &values);
    };

    /////////////////////////////////////
    // TABLE CLASS
    class Table {
        std::string name;
        std::deque<Column> columns;
        std::deque<Row> rows;
        int curr_column_id; // wewnetrzne id dla programu
        int curr_row_id;  // wewnetrzne id dla programu

    public:
        Table(const std::string &name);

        auto add_column(std::string name, DBType type);
        auto add_row(std::map<int, std::string> values);
        auto print();
    };
};
