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
                case lexer::TokenType::Quit:
                    parse_call_no_args();
                    return std::make_unique<ast::Quit>();
                    break;
                case lexer::TokenType::DBCreate:
                    return std::make_unique<ast::DBCreate>(parse_call_single_arg<ast::StringLiteral>());
                    break;
                default:
                    fmt::println("!!! Parser error: Unexpected tokentype while parsing node: {}",lexer::format_as(token_type));
            }
        }

        template<typename T>
        auto parse_expression() -> T {
            return parse_primary_expression<T>();
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
        auto parse_call_multiple_args() -> std::deque<ast::Expression> {
            auto args = std::deque<ast::Expression>();
            auto token = shift_token();
            if (token.getType() != lexer::TokenType::BracketRoundBegin) {
                fmt::println("!!! Error: Expected call for {}", lexer::format_as(get_prev_token()));
                throw;
            }
            while (shift_token().getType() != lexer::TokenType::BracketRoundEnd) {
                args.push_back(parse_expression<ast::Expression>());
            }
            shift_token();
            return args;
        }

        template<typename T>
        auto parse_primary_expression() -> T {
            auto token_type = get_token().getType();

            switch (token_type) {
//                case lexer::TokenType::Number:
//                    return ast::NumericLiteral(std::stoi(shift_token().getValue()));
//                    break;
                case lexer::TokenType::String:
                    return ast::StringLiteral(shift_token().getValue());
                    break;

                default:
                    fmt::println("!!! Parser error: Unexpected tokentype while parsing primary expression");
            }
        }

    public:
        Parser(std::string code) : tokens(lexer::tokenize(code)), point(tokens.begin()) {};

        auto produceAST() -> ast::Program {
            auto program = ast::Program();

            while (get_token().getType() != lexer::TokenType::EndOfFile) {
//                program.add_node(parse_node());
                program.getBody().emplace_back(parse_node());
            }

            return program;
        }
    };
}
