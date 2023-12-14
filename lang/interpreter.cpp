#include "interpreter.hpp"

Interpreter::Interpreter() : running(true), connected_to_db(false), database() {}

auto Interpreter::isRunning() -> bool const {
    return running;
}

auto Interpreter::quit() -> void{
    running = false;
}

auto Interpreter::runAST(ast::Program& program) -> void {
    std::unique_ptr<db::Table> curr_table;

    for (const auto& node : program.getBody()) {
        auto node_kind = node->getKind();

        switch (node_kind) {
            case ast::NodeType::Quit: {
                quit();
                break;
            }
            case ast::NodeType::DBCreate: {
                auto command = (ast::DBCreate *) node.get();
                database = db::create(command->getDbName().getValue());
                connected_to_db = true;
                break;
            }
            case ast::NodeType::KFCreateTable: {
                auto command = (ast::KFCreateTable *) node.get();
                database.create_table(command->getTableName().getValue());
                connected_to_db = true;
                break;
            }
            case ast::NodeType::KFGetTable: {
                auto command = (ast::KFGetTable*) node.get();
                if (connected_to_db) {
                    curr_table = std::make_unique<db::Table>(database.get_table(command->getTableName().getValue()));
                } else
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                break;
            }
            case ast::NodeType::KMAddColumn: {
                auto command = (ast::KMAddColumn*) node.get();
                if (!connected_to_db) {
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                    break;
                }
                if (!curr_table) {
                    fmt::println("!!! Interpreter error: add column used without chosen table");
                    break;
                }
                curr_table->add_column(
                        command->getName(),
                        db::Column::toColumnType(command->getType())
                        );
//                curr_table->add_row(std::vector<std::string>{"hahahah"});
//                curr_table->print();
                break;
            }
            default:
                fmt::println("!!! Interpreter error: unknown node type: {}", node_kind);
        }
    }
}

