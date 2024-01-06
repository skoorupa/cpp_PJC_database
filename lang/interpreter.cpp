#include "interpreter.hpp"

Interpreter::Interpreter() : running(true), connected_to_db(false), curr_database() {}

auto Interpreter::isRunning() -> bool const {
    return running;
}

auto Interpreter::quit() -> void{
    running = false;
}

auto Interpreter::runAST(ast::Program& program) -> void {
    db::Table* curr_table = nullptr;
    auto curr_result = db::Result();
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
                curr_database = db::create(command->getDbName().getValue());
                connected_to_db = true;
                break;
            }
            case ast::NodeType::KFCreateTable: {
                auto command = (ast::KFCreateTable *) node.get();
                curr_database.create_table(command->getTableName().getValue());
                connected_to_db = true;
                break;
            }
            case ast::NodeType::KFGetTable: {
                auto command = (ast::KFGetTable*) node.get();
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                try {
                    curr_table = &curr_database.get_table(command->getTableName().getValue());
                    curr_column = "";
                    curr_result.add_table(curr_database.get_table(command->getTableName().getValue()));
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

                auto column_type = db::toColumnType(command->getType());
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
                    try {
                        curr_database.rename_table(curr_table->getName(), new_name);
                    } catch (std::string& message) {
                        fmt::println("[DB ERROR] {}", message);
                    }
                }

                break;
            }
            case ast::NodeType::KMRemove: {
//                auto command = (ast::KMRemove*) node.get();

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
                } else if (curr_result.are_wheres_blank()) {
                    // removing table
                    try {
                        curr_database.remove_table(curr_table->getName());
                        curr_column = "";
                        curr_table = nullptr;
                    } catch (std::string& message) {
                        fmt::println("[DB ERROR] {}", message);
                    }
                } else if (curr_result.are_columns_blank()) {
                    // removing rows
                    try {
                        curr_result.remove_rows(curr_table);
                    } catch (std::string& message) {
                        fmt::println("[DB ERROR] {}", message);
                    }
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
            case ast::NodeType::KMUpdate: {
                auto command = (ast::KMUpdate*) node.get();

                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);
                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: select used without chosen table");

                try {
                    curr_result.update_rows(curr_table, command->getColumnName().getValue(), command->getValue());
                } catch (std::string& message) {
                    fmt::println("[DB ERROR] {}", message);
                }
                break;
            }
            case ast::NodeType::KMPrint: {
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: print used without chosen table");

                curr_result.print();
                break;
            }
            case ast::NodeType::KMSelect: {
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: select used without chosen table");

                auto command = (ast::KMSelect*)node.get();

                for (auto& expression : command->getExpressions()) {
                    switch (expression->getKind()) {
                        case ast::NodeType::Identifier: {
                            auto identifier = (ast::Identifier*) expression.get();
                            curr_result.add_column(curr_table->get_column(identifier->getSymbol()));
                            break;
                        }
                        default:
                            throw fmt::format("!!! Interpreter error: select did not expect following type {}", expression->getKind());
                    }
                }
                break;
            }
            case ast::NodeType::KMWhere: {
                if (!connected_to_db)
                    throw fmt::format("!!! Interpreter error: not connected to database in {}", node_kind);

                if (!curr_table)
                    throw fmt::format("!!! Interpreter error: where used without chosen table");

                auto command = (ast::KMWhere*)node.get();
                auto logicparser = db::LogicParser(command->getExpression().getTokens());
                try {
                    auto binary = logicparser.produce_logic_AST();
                    curr_result.add_where(binary);
                } catch (std::string s) {
                    throw s;
                }
                break;
            }
            default:
                throw fmt::format("!!! Interpreter error: unknown node type: {}", node_kind);
        }
    }
}

