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
}