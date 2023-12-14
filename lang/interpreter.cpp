#include "interpreter.hpp"

Interpreter::Interpreter() : running(true) {}

auto Interpreter::isRunning() -> bool const {
    return running;
}

auto Interpreter::quit() -> void{
    running = false;
}

auto Interpreter::runAST(ast::Program& program) -> void {
    for (const auto& node : program.getBody()) {
        auto node_kind = node->getKind();

        if (node_kind == ast::NodeType::DBCreate) {
            auto n = dynamic_cast<ast::DBCreate*>(node.get());
            fmt::println("casting - {}", n->getDbName().getValue());
        }
    }
}

