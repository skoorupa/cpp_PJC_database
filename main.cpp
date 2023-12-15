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

    for (std::string devcmd : {
        "create_db('baza')",
        "create_table('tablica')",
        "get_table('tablica') add_column('imie','string')",
        "get_table('tablica') add_column('nazwisko','string')",
        "get_table('tablica') add_row('adam','skorupski')",
        "get_table('tablica') add_row('kacper','kacperowski')",
        "get_table('tablica') print()"
    }) {
        auto parser = parser::Parser(devcmd);
        auto result = parser.produceAST();

        interpreter.runAST(result);
    }

    // ENDDEV

    while (interpreter.isRunning()) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

        auto parser = parser::Parser(prompt);
        auto result = parser.produceAST();

        interpreter.runAST(result);
    }
}
