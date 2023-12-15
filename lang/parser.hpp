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
        auto get_token() -> lexer::Token;
        auto get_prev_token() -> lexer::Token;
        auto shift_token() -> lexer::Token;
        auto parse_node() -> std::unique_ptr<ast::Node>;

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
        auto parse_expression_ptr() -> std::unique_ptr<ast::Expression>;
        auto parse_call_no_args() -> void;

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

        auto parse_call_multiple_args() -> std::deque<std::unique_ptr<ast::Expression>>;

    public:
        Parser(std::string code);

        auto produceAST() -> ast::Program;
    };
}
