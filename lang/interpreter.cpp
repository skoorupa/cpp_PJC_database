#include "interpreter.hpp"

Interpreter::Interpreter() : running(true), connected_to_db(false), database() {}

auto Interpreter::isRunning() -> bool const {
    return running;
}

auto Interpreter::quit() -> void{
    running = false;
}

auto Interpreter::runAST(ast::Program& program) -> void {
    db::Table* curr_table = nullptr;
    std::string curr_column;

    for (auto& node : program.getBody()) {
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
                if (!connected_to_db) {
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                    break;
                }
                try {
                    curr_table = &database.get_table(command->getTableName().getValue());
                } catch (std::string& name) {
                    fmt::println("!!! Interpreter error: cannot find table {}",name);
                }
                break;
            }
            case ast::NodeType::KMAddColumn: {
                auto command = (ast::KMAddColumn*) node.get();
                if (!connected_to_db) {
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                    break;
                }
                if (!curr_table) {
                    fmt::println("!!! Interpreter error: add_column used without chosen table");
                    break;
                }
                auto column_type = db::Column::toColumnType(command->getType());
                curr_table->add_column(
                        command->getName(),
                        column_type
                        );
                break;
            }
            case ast::NodeType::KMGetColumn: {
                auto command = (ast::KMGetColumn*) node.get();
                if (!connected_to_db) {
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                    break;
                }
                if (!curr_table) {
                    fmt::println("!!! Interpreter error: get_column used without chosen table");
                    break;
                }
                auto column_name = command->getColumnName().getValue();
                if (!curr_table->has_column(column_name)) {
                    fmt::println("!!! Interpreter error: table {} does not have column {}", curr_table->getName(), column_name);
                    break;
                }
                curr_column = column_name;
                break;
            }
            case ast::NodeType::KMAddRow: {
                auto command = (ast::KMAddRow*)node.get();
                if (!connected_to_db) {
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                    break;
                }
                if (!curr_table) {
                    fmt::println("!!! Interpreter error: add_row used without chosen table");
                    break;
                }
                curr_table->add_row(command->getValues());
                break;
            }
            case ast::NodeType::KMPrint: {
                if (!connected_to_db) {
                    fmt::println("!!! Interpreter error: not connected to database in {}", node_kind);
                    break;
                }
                if (!curr_table) {
                    fmt::println("!!! Interpreter error: print used without chosen table");
                    break;
                }
                curr_table->print();
                break;
            }
            default:
                fmt::println("!!! Interpreter error: unknown node type: {}", node_kind);
        }
    }
}

