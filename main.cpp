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
    auto quit = false;

    while (!quit) {
        auto prompt = std::string();
        fmt::print(">");
        std::getline(std::cin, prompt);

        if (prompt == "quit()" || prompt == "quit();")
            quit = true;
        else {
            auto parser = parser::Parser(prompt);
            auto result = parser.produceAST();

            for (const auto& node : result.getBody()) {
                if (node->getKind() == ast::NodeType::DBCreate) {
                    auto n = dynamic_cast<ast::DBCreate*>(node.get());
                    fmt::println("casting - {}", n->getDbName().getValue());
                }
            }
        }

//        fmt::println("{}", result);
    }
}
