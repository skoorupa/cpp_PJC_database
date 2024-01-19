#pragma once
#include <string>
#include <map>
#include <fmt/core.h>
#include <fmt/ranges.h>
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

        const std::string &getFilepath() const;
        const std::map<std::string, Table> &getTables() const;

        auto create() -> bool;
        auto connect() -> bool;
        auto create_table(std::string name) -> Table&;
        auto get_table(const std::string& name) -> Table&;
        auto rename_table(std::string old_name, std::string new_name) -> void;
        auto remove_table(const std::string& name) -> void;
        auto info() -> void;
        auto saver() -> std::string;
    };

    /////////////////////////////////////
    // DB HANDLING
    auto create(const std::string& filepath) -> Database;
    auto connect(const std::string& filepath) -> Database;
}
