#include <fmt/core.h>
#include "db/db.hpp"
#include "db/table.hpp"

auto main() -> int {
    auto test_db = db::create("test.db");
    db::Table emp0 = test_db.create_table("emp");
    db::Table& emp = test_db.get_table("emp");
    emp.add_column("id",db::ColumnType::Integer);
    emp.add_column("name",db::ColumnType::String);
    emp.add_row(
            std::vector<db::Value>{
                db::Value("1",db::ColumnType::Integer),
                db::Value("Adam",db::ColumnType::String)
            }
            );
    emp.add_row(
            std::vector<db::Value>{
                db::Value("2",db::ColumnType::Integer),
                db::Value("Martyna",db::ColumnType::String)
            }
            );
    emp.add_row(
            std::vector<db::Value>{
                db::Value("3",db::ColumnType::Integer),
                db::Value("Mikołaj",db::ColumnType::String)
            }
            );
    emp.add_row(
            std::vector<db::Value>{
                db::Value("4",db::ColumnType::Integer),
                db::Value("Ola",db::ColumnType::String)
            }
            );

    emp.print();
    emp.remove_column("name");
    emp.print();
}