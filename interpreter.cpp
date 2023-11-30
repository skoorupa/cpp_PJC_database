#pragma once
#include "interpreter.hpp"

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>

namespace interpreter {
    auto interpret_prompt(std::string input) -> std::vector<Token> {
        auto simple_token_vector = std::vector<Token>();
        auto token_value = std::string();
        auto closures_stack = std::stack<char>();
        auto current_tokentype = TokenType::Function;

        auto is_string = [closures_stack]() {
            return closures_stack.top()=='"' || closures_stack.top()=='\'';
        };
        auto is_expression = [closures_stack]() {
            return closures_stack.top()=='(';
        };

        for (auto c : input) {

            // building strings
            if (is_string() && c != closures_stack.top()) {
                token_value.push_back(c);
            }
            // closing strings
//            else if (
//                    (c == '"' || c == '\'') &&
//                    c == closures_stack.top()
//                    )
            else if (c == '(' && !is_string() && !is_expression()) {
                closures_stack.push('(');
                simple_token_vector.push_back(Token(current_tokentype, token_value));
                simple_token_vector.push_back(Token(TokenType::ClosureBegin, token_value));
                current_tokentype = TokenType::Expression;

                token_value = std::string();
            }

            token_value.push_back(c);

        }
//        simple_token_vector.push_back(token_value);

        return simple_token_vector;
    }

    ////////////////////////////////////////////

    Token::Token(TokenType type, const std::string &name) : type(type), name(name) {}

    std::map<char, char> closures = std::map<char, char>{
            {'(',')'},
            {'[',']'},
            {'{','}'},
            {'"','"'},
            {'\'','\''}
    };
    std::set<char> seperators = std::set<char>{'.',','};

    TokenType Token::getType() const {
        return type;
    }

    const std::string &Token::getName() const {
        return name;
    }

    auto tokentype_order = std::map<TokenType,std::vector<TokenType>>();
}
