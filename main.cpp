#include <fmt/core.h>
#include <string>
#include <iostream>

auto main() -> int {
    fmt::println("Welcome to Based - database in C++");
    auto quit = false;
    while (!quit) {
        auto prompt = std::string();
        fmt::print(">");
        std::cin >> prompt;

        fmt::println("{}",prompt);
    }
}
