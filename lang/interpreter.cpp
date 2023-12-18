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
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                try {
                    curr_table = &database.get_table(command->getTableName().getValue());
                    curr_column = "";
                } catch (std::string& message) {
                    throw fmt::format("[DB ERROR] {}",message);
                }
                break;
            }
            case ast::NodeType::KMAddColumn: {
                auto command = (ast::KMAddColumn*) node.get();
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: add_column used without chosen table");

                auto column_type = db::Column::toColumnType(command->getType());
                try {
                    curr_table->add_column(
                    command->getName(),
                    column_type
                    );
                } catch (std::string& message) {
                    fmt::println("[DB ERROR] {}", message);
                }
                break;
            }
            case ast::NodeType::KMGetColumn: {
                auto command = (ast::KMGetColumn*) node.get();
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: get_column used without chosen table");

                auto column_name = command->getColumnName().getValue();
                if (!curr_table->has_column(column_name))
                    throw fmt::format("!!! Interpreter error: table {} does not have column {}", curr_table->getName(), column_name);

                curr_column = column_name;
                break;
            }
            case ast::NodeType::KMRename: {
                auto command = (ast::KMRename*) node.get();
                auto new_name = command->getNewName().getValue();

                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                   throw fmt::format("!!! Interpreter error: rename used without chosen table");

                if (!curr_column.empty()) {
                    // renaming column

                    try {
                        curr_table->rename_column(curr_column, new_name);
                        curr_column = new_name;
                    } catch (std::string& message) {
                        fmt::println("[DB ERROR] {}", message);
                    }
                } else {
                    // renaming table
                    // TODO
                }

                break;
            }
            case ast::NodeType::KMRemove: {
                auto command = (ast::KMRemove*) node.get();

                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: remove used without chosen table");

                if (!curr_column.empty()) {
                    // removing column

                    try {
                        curr_table->remove_column(curr_column);
                        curr_column = "";
                    } catch (std::string& message) {
                        fmt::println("[DB ERROR] {}", message);
                    }
                } else {
                    // removing table
                    // TODO
                }

                break;
            }
            case ast::NodeType::KMAddRow: {
                auto command = (ast::KMAddRow*)node.get();
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: add_row used without chosen table");

                curr_table->add_row(command->getValues());
                break;
            }
            case ast::NodeType::KMPrint: {
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: print used without chosen table");

                curr_table->print();
                break;
            }
            default:
                throw fmt::format("!!! Interpreter error: unknown node type: {}", node_kind);
        }
    }
}

