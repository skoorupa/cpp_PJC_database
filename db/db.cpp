#include "db.hpp"

namespace db {
    /////////////////////////////////////
    // DATABASE CLASS
    Database::Database(const std::string &filepath) : filepath(filepath), tables(std::map<std::string, Table>()) {}
    Database::Database() : filepath(""), tables(std::map<std::string, Table>()) {}

    auto Database::create() -> bool {
        fmt::println("TEST - created new db {}",filepath);
        return true;
    }

    auto Database::connect() -> bool {
        fmt::println("TEST - connected to db {}",filepath);
        return true;
    }

    auto Database::create_table(std::string name) -> Table {
        tables.insert(std::pair<std::string, Table>(name, Table(name)));
        fmt::println("TEST - created table {}", name);
        return get_table(name);
    }

    auto Database::get_table(std::string name) -> Table {
        return tables.at(name);
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
