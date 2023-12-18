#include "db.hpp"

namespace db {
    /////////////////////////////////////
    // DATABASE CLASS
    Database::Database(const std::string &filepath) : filepath(filepath), tables(std::map<std::string, Table>()) {}
    Database::Database() : filepath(""), tables(std::map<std::string, Table>()) {}

    auto Database::create() -> bool {
        fmt::println("< created new database: {}",filepath);
        return true;
    }

    auto Database::connect() -> bool {
        fmt::println("< connected to database: {}",filepath);
        return true;
    }

    auto Database::create_table(std::string name) -> Table {
        tables.insert(std::pair<std::string, Table>(name, Table(name)));
        fmt::println("< created new table: {}", name);
        return get_table(name);
    }

    auto Database::get_table(const std::string& name) -> Table& {
//        return tables.at(name);
        auto pair = tables.find(name);
        if (pair != tables.end())
            return pair->second;
        else
            throw fmt::format("< cannot find table {}",name);
    }

    auto Database::rename_table(std::string old_name, std::string new_name) -> void {
        get_table(old_name).rename(new_name);
//      src:  https://stackoverflow.com/questions/5743545/what-is-the-fastest-way-to-change-a-key-of-an-element-inside-stdmap
        auto table = tables.extract(old_name);
        table.key() = new_name;
        tables.insert(std::move(table));
        fmt::println("< renamed table {} to {}", old_name, new_name);
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
