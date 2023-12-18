#pragma once
#include <string>
#include <deque>
#include <map>
#include <fmt/core.h>
#include "table.hpp"

namespace db {
    /////////////////////////////////////
    // DATABASE CLASS
    class Database {
        std::string filepath;
        std::map<std::string, Table> tables;

    public:
        Database(const std::string &filepath);
        Database();

        auto create() -> bool;
        auto connect() -> bool;
        auto create_table(std::string name) -> Table;
        auto get_table(const std::string& name) -> Table&;
        auto rename_table(std::string old_name, std::string new_name) -> void;
        auto remove_table(const std::string& name) -> void;
    };

    /////////////////////////////////////
    // DB HANDLING
    auto create(std::string filepath) -> Database;
    auto connect(std::string filepath) -> Database;
}
