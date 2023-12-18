#include <fmt/core.h>
#include "db/db.hpp"
#include "db/table.hpp"

auto main() -> int {
    auto test_db = db::create("test.db");
    test_db.create_table("emp");
    db::Table& emp = test_db.get_table("emp");
    emp.add_column("id",db::ColumnType::Integer);
    emp.add_column("imie",db::ColumnType::String);
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
                db::Value("Mikolaj",db::ColumnType::String)
            }
            );
    emp.add_row(
            std::vector<db::Value>{
                db::Value("4",db::ColumnType::Integer),
                db::Value("Ola",db::ColumnType::String)
            }
            );

    emp.print();
    emp.rename_column("id","zzz");
//    emp.remove_column("zzz");
    emp.print();
}