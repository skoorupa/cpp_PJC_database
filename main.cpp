#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include <string>
#include <iostream>

#include "lang/lexer.hpp"
#include "lang/parser.hpp"
#include "lang/interpreter.hpp"
#include "db/db.hpp"

auto main() -> int {
    fmt::println("Welcome to Based - database in C++");
    auto interpreter = Interpreter();

    // DEV

    for (const std::string& devcmd : {
        "create_db('baza')",
        "create_table(tablica)",
        "get_table(tablica) add_column(id,int)",
        "get_table(tablica) add_column(nazwisko,string,nullable)",
        "get_table(tablica) add_row(1,'skorupski')",
        "get_table(tablica) add_row(2,'kacperowski')",
        "get_table(tablica) print()",
        "get_table(tablica) add_column(test,string)",
        "get_table(tablica) print()",
        "get_table(tablica) get_column(id) rename(identyfikator) print()",
        "get_table(tablica) where(identyfikator==2) update(identyfikator, -1)",
        "get_table(tablica) select(tablica.nazwisko, identyfikator) print()",
        "info()",
        "create_table(tablica2)",
        "get_table(tablica2) add_column(id,int)",
        "get_table(tablica2) add_column(aaaaa,int)",
        "get_table(tablica2) add_row(1,10)",
        "get_table(tablica2) add_row(2,20)",
        "get_table(tablica,tablica2) print()",
        "create_table(tablica3)",
        "get_table(tablica3) add_column(id,int)",
        "get_table(tablica3) add_column(bbbbb,string)",
        "get_table(tablica3) add_row(111,'10')",
        "get_table(tablica3) add_row(222,'20')",
        "get_table(tablica,tablica2,tablica3) print()"
    }) {
        try {
            auto parser = parser::Parser(devcmd);
            auto result = parser.produceAST();

            interpreter.runAST(result);
        } catch (std::string& message) {
            fmt::println("{}", message);
        }
    }

    // ENDDEV

    while (interpreter.isRunning()) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

        try {
            auto parser = parser::Parser(prompt);
            auto result = parser.produceAST();

            interpreter.runAST(result);
        } catch (std::string& message) {
            fmt::println("{}", message);
        }
    }
}
