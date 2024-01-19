#include <fstream>
#include <filesystem>
#include "interpreter.hpp"

Interpreter::Interpreter() : running(true), connected_to_db(false), curr_database() {}

auto Interpreter::isRunning() -> bool const {
    return running;
}

auto Interpreter::quit() -> void{
    running = false;
}

auto Interpreter::runAST(ast::Program& program) -> void {
    auto curr_tables = std::vector<db::Table*>();
//    db::Table* curr_table = nullptr;
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

                namespace fs = std::filesystem;
                auto filepath = command->getDbName().getValue();
                if (fs::exists(filepath))
                    throw fmt::format("< database at path {} already exists, please use connect_db", filepath);

                auto dbstream = std::fstream(filepath, std::ios::out | std::ios::app);
                curr_database = db::create(filepath);
                connected_to_db = true;
                break;
            }
            case ast::NodeType::DBSave: {
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                auto filepath = curr_database.getFilepath();
                auto dbstream = std::fstream(filepath, std::ios::out | std::ios::app);

                dbstream << curr_database.saver();

                break;
            }
            case ast::NodeType::KFCreateTable: {
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                auto command = (ast::KFCreateTable *) node.get();
                curr_tables.push_back(&curr_database.create_table(command->getTableName().getValue()));
                connected_to_db = true;
                break;
            }
            case ast::NodeType::KFGetTable: {
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                auto command = (ast::KFGetTable*) node.get();
                auto args = command->getTableNames();
                auto tablenames = std::set<std::string>();

                for (auto& arg : args)
                    tablenames.insert(arg.getSymbol());

                for (auto& tablename : tablenames) {
                    try {
                        curr_tables.push_back(&curr_database.get_table(tablename));
                        curr_column = "";
                        curr_result.add_table(curr_database.get_table(tablename));
                    } catch (std::string& message) {
                        throw fmt::format("{}",message);
                    }
                }
                break;
            }
            case ast::NodeType::KMInfo: {
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty()) // DB INFO
                    curr_database.info();
                else if (curr_result.are_wheres_blank())
                    for (auto& table : curr_tables)
                        table->info();
                else
                    curr_result.info();

                break;
            }
            case ast::NodeType::KMAddColumn: {
                auto command = (ast::KMAddColumn*) node.get();
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< add_column used without chosen table");

                auto column_type = db::toColumnType(command->getType());
                try {
                    for (auto& table : curr_tables)
                        table->add_column(
                            command->getName(),
                            column_type,
                            command->isNullable()
                        );
                } catch (std::string& message) {
                    fmt::println("{}", message);
                }
                break;
            }
            case ast::NodeType::KMGetColumn: {
                auto command = (ast::KMGetColumn*) node.get();
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< get_column used without chosen table");

                auto column_name = command->getColumnName().getValue();
                for (auto& table : curr_tables)
                    if (!table->has_column(column_name))
                        throw fmt::format("< table {} does not have column {}", table->getName(), column_name);

                curr_column = column_name;
                break;
            }
            case ast::NodeType::KMRename: {
                auto command = (ast::KMRename*) node.get();
                auto new_name = command->getNewName().getValue();

                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                   throw fmt::format("< rename used without chosen table");

                if (!curr_column.empty()) {
                    // renaming column
                    try {
                        for (auto& table : curr_tables)
                            table->rename_column(curr_column, new_name);
                        curr_column = new_name;
                    } catch (std::string& message) {
                        fmt::println("{}", message);
                    }
                } else {
                    // renaming table
                    if (curr_tables.size() != 1)
                        throw fmt::format("< cannot rename multiple tables at once");

                    try {
                        curr_database.rename_table(curr_tables.at(0)->getName(), new_name);
                    } catch (std::string& message) {
                        fmt::println("{}", message);
                    }
                }

                break;
            }
            case ast::NodeType::KMRemove: {
//                auto command = (ast::KMRemove*) node.get();

                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< remove used without chosen table");

                if (!curr_column.empty()) {
                    // removing column

                    try {
                        for (auto& table : curr_tables)
                            table->remove_column(curr_column);
                        curr_column = "";
                    } catch (std::string& message) {
                        fmt::println("{}", message);
                    }
                } else if (curr_result.are_wheres_blank()) {
                    // removing table
                    try {
                        for (auto& table : curr_tables) {
                            curr_database.remove_table(table->getName());
                            curr_tables.erase(std::ranges::find(curr_tables, table));
                        }
                        curr_column = "";
                    } catch (std::string& message) {
                        fmt::println("{}", message);
                    }
                } else if (curr_result.are_columns_blank()) {
                    // removing rows
                    if (curr_tables.size() != 1)
                        throw fmt::format("< cannot remove rows from multiple tables");

                    try {
                        curr_result.remove_rows(curr_tables.at(0));
                    } catch (std::string& message) {
                        fmt::println("{}", message);
                    }
                }

                break;
            }
            case ast::NodeType::KMAddRow: {
                auto command = (ast::KMAddRow*)node.get();
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< add_row used without chosen table");

                for (auto& table : curr_tables)
                    table->add_row(command->getValues());
                break;
            }
            case ast::NodeType::KMUpdate: {
                auto command = (ast::KMUpdate*) node.get();

                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);
                if (curr_tables.empty())
                    throw fmt::format("< select used without chosen table");

                try {
                    for (auto& table : curr_tables)
                        curr_result.update_rows(table, command->getColumnName().getValue(), command->getValue());
                } catch (std::string& message) {
                    fmt::println("{}", message);
                }
                break;
            }
            case ast::NodeType::KMPrint: {
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< print used without chosen table");

                curr_result.print();
                break;
            }
            case ast::NodeType::KMSelect: {
                if (!connected_to_db)
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< select used without chosen table");

                auto command = (ast::KMSelect*)node.get();

                for (auto& expression : command->getExpressions()) {
                    switch (expression->getKind()) {
                        case ast::NodeType::Identifier: {
                            auto identifier = (ast::Identifier*) expression.get();
                            auto arg = identifier->getSymbol();
                            auto columnname = std::string();
                            auto dot = arg.find('.');
                            if (arg.find('.')<arg.length()) {
                                // kolumna ma określoną tablicę
                                columnname = arg.substr(dot+1, arg.length());
                                auto tablename = arg.substr(0, dot);

                                auto table = std::ranges::find_if(
                                    curr_tables,
                                    [tablename](db::Table*& table)->bool{
                                        return table->getName() == tablename;
                                    });
                                if (table != curr_tables.end())
                                    curr_result.add_column((*table)->get_column(columnname));
                                else
                                    throw fmt::format("< cannot find column {} in table {}",columnname, tablename);
                            } else {
                                columnname = arg;
                                auto occurences = std::ranges::count_if(
                                    curr_tables,
                                    [columnname](db::Table*& table)->bool {
                                        return table->has_column(columnname);
                                    });

                                if (occurences == 0)
                                    throw fmt::format("< cannot find column {} in query", columnname);
                                else if (occurences > 1)
                                    throw fmt::format("< there are many columns named {} in query, please use <table>.<column> syntax", columnname);

                                auto table = std::ranges::find_if(
                                    curr_tables,
                                    [columnname](db::Table*& table)->bool {
                                        return table->has_column(columnname);
                                    }
                                );
                                curr_result.add_column((*table)->get_column(columnname));
                            }

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
                    throw fmt::format("< not connected to database in {}", node_kind);

                if (curr_tables.empty())
                    throw fmt::format("< where used without chosen table");

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

