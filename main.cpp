#include <fmt/core.h>
#include <fmt/ranges.h>
#include <string>
#include <iostream>

#include "interpreter.hpp"

auto main() -> int {
    fmt::println("Welcome to Based - database in C++");
    auto quit = false;

    while (!quit) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

//        fmt::println("{}", interpreter::interpret_prompt(prompt));
    }
}
