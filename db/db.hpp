#pragma once
#include <string>
#include <deque>
#include <map>
#include "table.hpp"

namespace db {
    /////////////////////////////////////
    // DATABASE CLASS
    class Database {
        std::string filepath;
        std::map<std::string, Table> tables;

    public:
        Database(const std::string &filepath);

        auto create();
        auto connect();
        auto create_table(std::string name) -> Table;
        auto get_table(std::string name) -> Table;
    };

    /////////////////////////////////////
    // DB HANDLING
    auto create(std::string filepath) -> Database;
    auto connect(std::string filepath) -> Database;
}
