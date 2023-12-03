#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include <string>
#include <iostream>

#include "lang/lexer.hpp"
#include "lang/parser.hpp"
#include "db/db.hpp"

auto main() -> int {
    fmt::println("Welcome to Based - database in C++");
    auto parser = parser::Parser();
    auto quit = false;

    while (!quit) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

        auto result = parser.produceAST(prompt);

        fmt::println("{}", result);
    }
}
