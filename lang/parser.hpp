#include "ast.hpp"
#include "lexer.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include <memory>

namespace parser {
    class Parser {
        std::deque<lexer::Token> tokens;
        std::deque<lexer::Token>::iterator point; // o tym sposobie trzymania iteratora dowiedzialem sie z GPT

        auto get_point (int i = 0) {
            return point+i;
        }
        auto get_token() -> lexer::Token {
            return *get_point();
        }
        auto get_prev_token() {
            return *get_point(-1);
        }
        auto shift_token() -> lexer::Token {
            auto token = get_token();
            point++;
            return token;
        }
        auto parse_node() -> std::unique_ptr<ast::Node> {
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

        template<typename T>
        auto parse_expression() -> T {
            return parse_primary_expression<T>();
        }
        template<typename T>
        auto parse_primary_expression() -> T {
            auto token_type = get_token().getType();

            switch (token_type) {
//                case lexer::TokenType::Number:
//                    return ast::NumericLiteral(std::stoi(shift_token().getValue()));
//                    break;
                case lexer::TokenType::String: {
                    return ast::StringLiteral(shift_token().getValue());
                    break;
                }

                default:
                    fmt::println("!!! Parser error: Unexpected tokentype while parsing primary expression: {}",token_type);
            }
        }
        auto parse_expression_ptr() -> std::unique_ptr<ast::Expression> {
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
        auto parse_call_no_args() -> void {
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
        template <typename T>
        auto parse_call_single_arg() -> T {
            auto token = shift_token();
            if (token.getType() != lexer::TokenType::BracketRoundBegin) {
                fmt::println("!!! Error: Expected call for {}", get_prev_token());
                fmt::println("got: {}", token);
                throw;
            }
            auto expression = parse_expression<T>();
            if (shift_token().getType() != lexer::TokenType::BracketRoundEnd) {
                fmt::println("!!! Error: Expected one argument for {}", get_prev_token());
                throw;
            }
            return expression;
        }
        auto parse_call_multiple_args() -> std::deque<std::unique_ptr<ast::Expression>> {
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

    public:
        Parser(std::string code) : tokens(lexer::tokenize(code)), point(tokens.begin()) {};

        auto produceAST() -> ast::Program {
            auto program = ast::Program();

//            for (lexer::Token token : tokens) {
//                fmt::println("{}",token);
//            }

            while (get_token().getType() != lexer::TokenType::EndOfFile) {
                program.getBody().emplace_back(parse_node());
            }

            return program;
        }
    };
}
