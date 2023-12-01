#include <string>

namespace db {
    // DATABASE CLASS
    class Database {
        std::string filepath;

    public:
        Database(const std::string &filepath);

        auto create();
        auto connect();
    };

    // DB HANDLING
    auto create(std::string filepath);
}
