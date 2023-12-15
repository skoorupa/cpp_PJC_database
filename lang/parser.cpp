#include "parser.hpp"

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
                return std::make_unique<ast::DBCreate>(parse_call_single_arg<ast::StringLiteral>());
                break;
            }
            case lexer::TokenType::KFCreateTable: {
                return std::make_unique<ast::KFCreateTable>(parse_call_single_arg<ast::StringLiteral>());
                break;
            }
            case lexer::TokenType::KFGetTable: {
                return std::make_unique<ast::KFGetTable>(parse_call_single_arg<ast::StringLiteral>());
                break;
            }
            case lexer::TokenType::KMAddColumn: {
                auto args = parse_call_multiple_args();
                return std::make_unique<ast::KMAddColumn>(
                        ((ast::StringLiteral*)args.at(0).get())->getValue(),
                        ((ast::Identifier*)args.at(1).get())->getSymbol()
                );
                break;
            }
            case lexer::TokenType::KMAddRow: {
                auto args = parse_call_multiple_args();
                auto values = std::vector<std::string>();
                for (const auto& arg : args) {
                    if (arg->getKind() == ast::NodeType::StringLiteral) {
                        values.push_back(((ast::StringLiteral*)arg.get())->getValue());
                    }
                }
                return std::make_unique<ast::KMAddRow>(
                        values
                );
                break;
            }
            case lexer::TokenType::KMPrint: {
                parse_call_no_args();
                return std::make_unique<ast::KMPrint>();
                break;
            }
            default:
                fmt::println("!!! Parser error: Unexpected tokentype while parsing node: {}",lexer::format_as(token_type));
        }
    }

    auto Parser::parse_expression_ptr() -> std::unique_ptr<ast::Expression> {
        auto token_type = get_token().getType();
        auto expr = std::unique_ptr<ast::Expression>();

        while (token_type != lexer::TokenType::CommaOperator && token_type != lexer::TokenType::BracketRoundEnd) {
            switch (token_type) {
                //                case lexer::TokenType::Number:
                //                    return ast::NumericLiteral(std::stoi(shift_token().getValue()));
                //                    break;
                case lexer::TokenType::Identifier: {
                    expr = std::make_unique<ast::Identifier>(shift_token().getValue());
                    break;
                }
                case lexer::TokenType::String: {
                    expr = std::make_unique<ast::StringLiteral>(shift_token().getValue());
                    break;
                }

                default:
                    fmt::println("!!! Parser error: Unexpected tokentype while parsing expression ptr: {}", token_type);
                    throw;
            }
            token_type = get_token().getType();
        }
        if (token_type == lexer::TokenType::CommaOperator)
            shift_token();

        return expr;
    }

    auto Parser::parse_call_no_args() -> void {
        auto token = shift_token();
        if (token.getType() != lexer::TokenType::BracketRoundBegin) {
            fmt::println("!!! Error: Expected call for {}", get_prev_token());
            fmt::println("got: {}", token);
            throw;
        }
        if (shift_token().getType() != lexer::TokenType::BracketRoundEnd) {
            fmt::println("!!! Error: Expected one argument for {}", get_prev_token());
            throw;
        }
    }

    auto Parser::parse_call_multiple_args() -> std::deque<std::unique_ptr<ast::Expression>> {
        auto args = std::deque<std::unique_ptr<ast::Expression>>();
        auto token = shift_token();
        if (token.getType() != lexer::TokenType::BracketRoundBegin) {
            fmt::println("!!! Error: Expected call for {}", get_prev_token());
            throw;
        }

        while (get_token().getType() != lexer::TokenType::BracketRoundEnd) {
            args.emplace_back(parse_expression_ptr());
        }
        shift_token();
        return args;
    }

    // PUBLIC

    Parser::Parser(std::string code) : tokens(lexer::tokenize(code)), point(tokens.begin()) {};

    auto Parser::produceAST() -> ast::Program {
        auto program = ast::Program();

//            for (lexer::Token token : tokens) {
//                fmt::println("{}",token);
//            }

        while (get_token().getType() != lexer::TokenType::EndOfFile) {
            program.getBody().emplace_back(parse_node());
        }

        return program;
    }
}