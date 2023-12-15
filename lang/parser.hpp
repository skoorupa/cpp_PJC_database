#include "ast.hpp"
#include "lexer.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include <memory>
#include <string>

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

        auto parse_expression() -> std::unique_ptr<ast::Expression> {
            return parse_primary_expression();
        }
        auto parse_primary_expression() -> std::unique_ptr<ast::Expression> {
            return parse_expression_ptr();
        }
        auto parse_expression_ptr() -> std::unique_ptr<ast::Expression>;
        auto parse_call_no_args() -> void;

        auto parse_call_single_arg() -> std::unique_ptr<ast::Expression> {
            auto token = shift_token();
            if (token.getType() != lexer::TokenType::BracketRoundBegin) {
                fmt::println("!!! Error: Expected call for {}", get_prev_token());
                fmt::println("got: {}", token);
                throw;
            }
            auto expression = parse_expression();
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
