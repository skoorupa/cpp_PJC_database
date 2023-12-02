#include <fmt/core.h>
#include "db/db.hpp"
#include "db/table.hpp"

auto main() -> int {
    auto test_db = db::create("test.db");
    db::Table emp = test_db.create_table("emp");
    emp.add_column("id",db::DBType::Integer);
    emp.add_column("name",db::DBType::String);
    emp.add_row(std::vector<std::string>{"1", "Adam"});
    emp.add_row(std::vector<std::string>{"2", "Karolina"});
    emp.add_row(std::vector<std::string>{"3", "Mikołaj"});
    emp.add_row(std::vector<std::string>{"4", "Ola"});

    emp.print();
}