#include <string>

namespace db {
    /////////////////////////////////////
    // DBTYPES
    enum class ColumnType {
        Integer, String
    };

    /////////////////////////////////////
    // COLUMN CLASS
    class Column {
        std::string name;
        ColumnType type;
    public:
        Column(const std::string &name, ColumnType type);

        int getId() const;
        const std::string &getName() const;
        ColumnType getType() const;
    };
}