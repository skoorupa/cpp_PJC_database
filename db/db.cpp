//
// Created by adams on 01.12.2023.
//

#include "table.hpp"
#include "db.hpp"

namespace db {
    /////////////////////////////////////
    // DATABASE CLASS
    Database::Database(const std::string &filepath) : filepath(filepath), tables(std::deque<Table>()) {}

    auto Database::create() {
        return nullptr;
    }

    auto Database::connect() {
        return nullptr;
    }

    /////////////////////////////////////
    // DB HANDLING
    auto create(std::string filepath) -> Database {
        auto db_new = Database(filepath);
        db_new.create();
        return db_new;
    }

    auto connect(std::string filepath) -> Database {
        auto db_new = Database(filepath);
        db_new.connect();
        return db_new;
    }
}
