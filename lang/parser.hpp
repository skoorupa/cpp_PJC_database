#include "ast.hpp"
#include "lexer.hpp"

namespace parser {
    class Parser {
        std::deque<lexer::Token> tokens;

        auto not_EOF() -> bool;
        auto get_token() -> lexer::Token;
        auto shift_token() -> lexer::Token;
        auto parse_node() -> ast::Node;
        auto parse_expression() -> ast::Expression;
        auto parse_primary_expression() -> ast::Expression;

    public:
        Parser();

        auto produceAST(std::string code) -> ast::Program;
    };

}
