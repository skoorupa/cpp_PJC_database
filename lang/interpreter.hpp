#pragma once
#include "ast.hpp"
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

class Interpreter {
    bool running;

public:
    Interpreter();
    auto isRunning() -> bool const;
    auto quit() -> void;

    auto runAST(ast::Program& program) -> void;
};
