#include "interpreter.hpp"

Interpreter::Interpreter() : running(true), database() {}

auto Interpreter::isRunning() -> bool const {
    return running;
}

auto Interpreter::quit() -> void{
    running = false;
}

auto Interpreter::runAST(ast::Program& program) -> void {
    for (const auto& node : program.getBody()) {
        auto node_kind = node->getKind();

        switch (node_kind) {
            case ast::NodeType::Quit:
                quit();
                break;
            case ast::NodeType::DBCreate:
                auto command = dynamic_cast<ast::DBCreate*>(node.get());
                database = db::create(command->getDbName().getValue());
                break;
        }
    }
}

