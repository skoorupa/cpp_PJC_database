#pragma once
#include "ast.hpp"
#include "../db/db.hpp"
#include "../db/table.hpp"
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

class Interpreter {
    bool running;
    bool connected_to_db;
    db::Database curr_database;

public:
    Interpreter();
    auto isRunning() -> bool const;
    auto quit() -> void;

    auto runAST(ast::Program& program) -> void;
};
