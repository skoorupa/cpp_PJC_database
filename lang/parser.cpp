#include "parser.hpp"

namespace parser {
    Parser::Parser() {
        tokens = std::deque<lexer::Token>();
    }

    auto Parser::not_EOF() -> bool {
        return tokens.front().getType() != lexer::TokenType::EndOfFile;
    }

    auto Parser::get_token() -> lexer::Token {
        return tokens.front();
    }
    auto Parser::shift_token() -> lexer::Token {
        auto token = get_token();
        tokens.pop_front();
        return token;
    }

    auto Parser::parse_statement() -> ast::Node {
        return parse_expression();
    }

    auto Parser::parse_expression() -> ast::Expression {
        return parse_primary_expression();
    }

    auto Parser::parse_primary_expression() -> ast::Expression {
        auto token_type = get_token().getType();

        switch (token_type) {
            case lexer::TokenType::Identifier:
                return ast::Identifier(shift_token().getValue());
                break;
            case lexer::TokenType::Number:
                return ast::NumericLiteral(std::stoi(shift_token().getValue()));
                break;

            default:
                fmt::println("!!! Parser error: Unexpected tokentype ({})",lexer::format_as(get_token()));
        }
    }

    auto Parser::produceAST(std::string code) -> ast::Program {
        tokens = lexer::tokenize(code);
        auto program = ast::Program();

        while (not_EOF()) {
            program.add_statement(parse_statement());
        }

        return program;
    }
}