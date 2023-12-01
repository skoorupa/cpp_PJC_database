#include <string>
#include <deque>

namespace db {
    /////////////////////////////////////
    // DATABASE CLASS
    class Database {
        std::string filepath;
        std::deque<Table> tables;

    public:

        auto create();
        auto connect();

        Database(const std::string &filepath);
    };

    /////////////////////////////////////
    // DB HANDLING
    auto create(std::string filepath) -> Database;
    auto connect(std::string filepath) -> Database;
}
