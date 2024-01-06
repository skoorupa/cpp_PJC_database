#include "parser.hpp"
#include "../db/column.hpp"

namespace parser {
    auto Parser::get_token() -> lexer::Token {
        return *Parser::get_point();
    };
    auto Parser::get_prev_token() -> lexer::Token {
        return *get_point(-1);
    }
    auto Parser::shift_token() -> lexer::Token {
        auto token = get_token();
        point++;
        return token;
    }
    auto Parser::parse_node() -> std::unique_ptr<ast::Node> {
        auto token_type = shift_token().getType();

        switch (token_type) {
            case lexer::TokenType::Quit: {
                parse_call_no_args();
                return std::make_unique<ast::Quit>();
                break;
            }
            case lexer::TokenType::DBCreate: {
                auto arg = parse_call_single_arg();
                return std::make_unique<ast::DBCreate>(((ast::Identifier*)arg.get())->getSymbol());
                break;
            }
            case lexer::TokenType::KFCreateTable: {
                auto arg = parse_call_single_arg();
                return std::make_unique<ast::KFCreateTable>(((ast::Identifier*)arg.get())->getSymbol());
                break;
            }
            case lexer::TokenType::KFGetTable: {
                auto arg = parse_call_single_arg();
                return std::make_unique<ast::KFGetTable>(((ast::Identifier*)arg.get())->getSymbol());
                break;
            }
            case lexer::TokenType::KMAddColumn: {
                auto args = parse_call_multiple_args();
                auto identifiers = std::vector<ast::Identifier*>();

                for (auto& arg : args) {
                    identifiers.push_back((ast::Identifier*)arg.get());
                }

                auto nullable = std::ranges::find_if(
                        identifiers.begin()+2,
                        identifiers.end(),
                        [](auto item){return item->getSymbol() == "nullable";})
                                != identifiers.end();

                return std::make_unique<ast::KMAddColumn>(
                        identifiers[0]->getSymbol(),
                        identifiers[1]->getSymbol(),
                        nullable
                );
                break;
            }
            case lexer::TokenType::KMRename: {
                auto arg = parse_call_single_arg();
                return std::make_unique<ast::KMRename>(
                        ((ast::Identifier*)arg.get())->getSymbol()
                );
                break;
            }
            case lexer::TokenType::KMRemove: {
                parse_call_no_args();
                return std::make_unique<ast::KMRemove>();
                break;
            }
            case lexer::TokenType::KMGetColumn: {
                auto arg = parse_call_single_arg();
                return std::make_unique<ast::KMGetColumn>(((ast::Identifier*)arg.get())->getSymbol());
                break;
            }
            case lexer::TokenType::KMAddRow: {
                auto args = parse_call_multiple_args();
                auto values = std::vector<db::Value>();
                for (const auto& arg : args) {
                    if (arg->getKind() == ast::NodeType::StringLiteral) {
                        values.push_back(db::Value(
                                ((ast::StringLiteral*)arg.get())->getValue(),
                                db::ColumnType::String
                                ));
                    } else if (arg->getKind() == ast::NodeType::NumericLiteral) {
                        values.push_back(db::Value(
                                std::to_string(((ast::NumericLiteral*)arg.get())->getValue()),
                                db::ColumnType::Integer
                        ));
                    } else if (arg->getKind() == ast::NodeType::NullLiteral) {
                        values.push_back(db::Value(
                                "null",
                                db::ColumnType::Null
                        ));
                    }
                }
                return std::make_unique<ast::KMAddRow>(
                        values
                );
                break;
            }
            case lexer::TokenType::KMUpdate: {
                auto args = parse_call_multiple_args();

                auto value = db::Value("null", db::ColumnType::Null);
                if (args[1]->getKind() == ast::NodeType::StringLiteral) {
                    value = db::Value(
                            ((ast::StringLiteral*)args[1].get())->getValue(),
                            db::ColumnType::String
                    );
                } else if (args[1]->getKind() == ast::NodeType::NumericLiteral) {
                    value = db::Value(
                            std::to_string(((ast::NumericLiteral*)args[1].get())->getValue()),
                            db::ColumnType::Integer
                    );
                } else if (args[1]->getKind() == ast::NodeType::NullLiteral) {
                    value = db::Value(
                            "null",
                            db::ColumnType::Null
                    );
                }
                return std::make_unique<ast::KMUpdate>(
                        ((ast::Identifier*)args.at(0).get())->getSymbol(),
                        value
                );
                break;
            }
            case lexer::TokenType::KMPrint: {
                parse_call_no_args();
                return std::make_unique<ast::KMPrint>();
                break;
            }
            case lexer::TokenType::KMSelect: {
                auto args = parse_call_multiple_args();
                return std::make_unique<ast::KMSelect>(std::move(args));
                break;
            }
            case lexer::TokenType::KMWhere: {
                auto chain = parse_call_token_chain();
                auto logic_chain = ast::LogicalChainExpression(chain);
                return std::make_unique<ast::KMWhere>(logic_chain);
                break;
            }
            default:
                throw fmt::format("!!! Parser error: Unexpected tokentype while parsing node: {}",lexer::format_as(token_type));
        }
    }

    auto Parser::parse_primary_expression_ptr() -> std::unique_ptr<ast::Expression> {
        auto token_type = get_token().getType();
        std::unique_ptr<ast::Expression> expr = nullptr;

        while (token_type != lexer::TokenType::CommaOperator && token_type != lexer::TokenType::BracketRoundEnd) {
            expr = parse_expression_ptr();
            token_type = get_token().getType();
        }
        if (token_type == lexer::TokenType::CommaOperator)
            shift_token();

        return expr;
    }

    auto Parser::parse_expression_ptr() -> std::unique_ptr<ast::Expression> {
        auto token_type = get_token().getType();
        std::unique_ptr<ast::Expression> expr = nullptr;

        switch (token_type) {
            case lexer::TokenType::Number: {
                expr = std::make_unique<ast::NumericLiteral>(std::stoi(shift_token().getValue()));
                break;
            }
            case lexer::TokenType::Identifier: {
                expr = std::make_unique<ast::Identifier>(shift_token().getValue());
                break;
            }
            case lexer::TokenType::String: {
                expr = std::make_unique<ast::StringLiteral>(shift_token().getValue());
                break;
            }
            case lexer::TokenType::Null: {
                shift_token();
                expr = std::make_unique<ast::NullLiteral>();
                break;
            }
            default:
                throw fmt::format("!!! Parser error: Unexpected tokentype while parsing expression ptr: {}", token_type);
        }
        token_type = get_token().getType();
        return expr;
    }

    auto Parser::parse_call_no_args() -> void {
        auto caller = get_prev_token();
        auto token = shift_token();
        if (token.getType() != lexer::TokenType::BracketRoundBegin)
            throw fmt::format("!!! Parser error: Expected call for {}", caller);

        if (shift_token().getType() != lexer::TokenType::BracketRoundEnd)
            throw fmt::format("!!! Parser error: Expected no arguments for {}", caller);
    }

    auto Parser::parse_call_single_arg() -> std::unique_ptr<ast::Expression> {
        auto token = shift_token();
        if (token.getType() != lexer::TokenType::BracketRoundBegin)
            throw fmt::format("!!! Parser error: Expected call for {}, got {}", get_prev_token(), token);

        auto expression = parse_primary_expression_ptr();
        if (shift_token().getType() != lexer::TokenType::BracketRoundEnd)
            throw fmt::format("!!! Parser error: Expected one argument for {}", get_prev_token());

        return expression;
    }

    auto Parser::parse_call_multiple_args() -> std::vector<std::unique_ptr<ast::Expression>> {
        auto args = std::vector<std::unique_ptr<ast::Expression>>();
        auto token = shift_token();
        if (token.getType() != lexer::TokenType::BracketRoundBegin)
            throw fmt::format("!!! Parser error: Expected call for {}", get_prev_token());

        while (get_token().getType() != lexer::TokenType::BracketRoundEnd)
            args.emplace_back(parse_primary_expression_ptr());

        shift_token();
        return args;
    }

    auto Parser::parse_call_token_chain() -> std::vector<lexer::Token> {
        shift_token();
        auto token_type = get_token().getType();
        std::vector<lexer::Token> chain = std::vector<lexer::Token>();

        while (token_type != lexer::TokenType::BracketRoundEnd) {
            chain.push_back(shift_token());
            token_type = get_token().getType();
        }
        shift_token();
        return chain;
    }

    // PUBLIC

    Parser::Parser(std::string code) : tokens(lexer::tokenize(code)), point(tokens.begin()) {};

    auto Parser::produceAST() -> ast::Program {
        auto program = ast::Program();
        // DEV: print tokens
//        for (lexer::Token token : tokens) {
//            fmt::println("{}",token);
//        }

        while (get_token().getType() != lexer::TokenType::EndOfFile) {
            program.getBody().emplace_back(parse_node());
        }

        return program;
    }
}