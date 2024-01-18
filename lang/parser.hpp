#pragma once

#include "ast.hpp"
#include "lexer.hpp"
#include "fmt/core.h"
#include "fmt/format.h"
#include <memory>
#include <string>

namespace parser {
    class Parser {
        std::vector<lexer::Token> tokens;
        std::vector<lexer::Token>::iterator point;

        auto get_point (int i = 0) {
            return point+i;
        }
        auto get_token() -> lexer::Token;
        auto get_prev_token() -> lexer::Token;
        auto shift_token() -> lexer::Token;

        auto parse_node() -> std::unique_ptr<ast::Node>;
        auto parse_primary_expression_ptr() -> std::unique_ptr<ast::Expression>;
        auto parse_expression_ptr() -> std::unique_ptr<ast::Expression>;

        auto parse_call_no_args() -> void;
        auto parse_call_single_arg() -> std::unique_ptr<ast::Expression>;
        auto parse_call_multiple_args() -> std::vector<std::unique_ptr<ast::Expression>>;
        auto parse_call_token_chain() -> std::vector<lexer::Token>;

    public:
        Parser(std::string code);

        auto produceAST() -> ast::Program;
    };
}
