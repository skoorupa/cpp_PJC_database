#include <fmt/core.h>
#include <fmt/ranges.h>
#include <string>
#include <iostream>
#include <vector>

struct query_element {

};

auto main() -> int {
    fmt::println("Welcome to Based - database in C++");
    auto quit = false;
    while (!quit) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

        auto split_prompt = std::vector<std::string>();
        auto prompt_part = std::string();

        for (auto c : prompt) {
            if (c == '"') {
                split_prompt.push_back(prompt_part);
                prompt_part = std::string();
            }

            prompt_part.push_back(c);
        }
        split_prompt.push_back(prompt_part);

        fmt::println("{}", split_prompt);
    }
}
