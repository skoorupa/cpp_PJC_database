// napisane z pomocą https://www.youtube.com/watch?v=8VB5TY1sIRo

#pragma once
#include "lexer.hpp"

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <fmt/core.h>
#include <fmt/format.h>


namespace lexer {
    auto tokenize(std::string input) -> std::vector<Token> {
        fmt::println("starting to tokenize");
        // HELPFUL VARIABLES
        auto closures = std::map<char, char>{
                {'(',')'},
                {'[',']'},
                {'{','}'},
                {'"','"'},
                {'\'','\''}
        };
        auto seperators = std::set<char>{'.',','};
        auto keywords = std::map<std::string, lexer::TokenType>{
                {"db_connect", TokenType::DBConnect},
                {"db_create", TokenType::DBCreate},
                {"get_table", TokenType::KFGetTable},
                {"select", TokenType::KMSelect},
                {"where", TokenType::KMWhere}
        };
        auto skip = std::set<char>{' ','\n','\t'};
        auto isskip = [skip](char c) {
            return skip.contains(c);
        };

        // TOKENIZER
        auto tokens = std::vector<Token>();

        auto pop_front_str = [](std::string& input) {
            input.erase(0,1);
        };

        while (!input.empty()) {
            auto c = input.begin();
            if (*c=='(') {
                tokens.push_back(Token(TokenType::BracketRoundBegin, "("));
                pop_front_str(input);
            }
            else if (*c==')') {
                tokens.push_back(Token(TokenType::BracketRoundEnd, ")"));
                pop_front_str(input);
            } else {
                // handle multicharacter tokens

                // number token
                if (isdigit(*c)) {
                    auto num = std::string();
                    while (!input.empty() && isdigit(*c)) {
                        num += *c;
                        pop_front_str(input);
                        fmt::println("{}",*c);
                    }

                    tokens.push_back(Token(TokenType::Number, num));
                }
                // keyword token
                else if (isalpha(*c) || *c == '_') {
                    auto txt = std::string();
                    while (!input.empty() && isalpha(*c)) {
                        txt += *c;
                        pop_front_str(input);
                    }

                    // look for keywords
                    auto iskeyword = keywords.contains(txt);

                    if (iskeyword) {
                        tokens.push_back(Token(keywords.at(txt), txt));
                    } else {
                        tokens.push_back(Token(TokenType::KUndefined, txt));
                    }
                } else if (isskip(*c)) {
                    pop_front_str(input);
                } else {
                    fmt::println("!!! Lexer error: cannot recognize character: {}",*c);
                    return tokens;
                }
            }
        }

        fmt::println("stopped tokenizing");
        return tokens;
    }

    ////////////////////////////////////////////

    Token::Token(TokenType type, const std::string &value) : type(type), value(value) {}

    TokenType Token::getType() const {
        return type;
    }

    const std::string &Token::getValue() const {
        return value;
    }

    auto format_as(Token token) -> std::string {
        auto tokentype_map = std::map<lexer::TokenType, std::string>{
                {lexer::TokenType::BracketRoundBegin, "BracketRoundBegin"},
                {lexer::TokenType::BracketRoundEnd, "BracketRoundEnd"},
                {lexer::TokenType::Number, "Number"},
                {lexer::TokenType::KFGetTable, "KFGetTable"},
                {lexer::TokenType::KMSelect, "KMSelect"},
                {lexer::TokenType::KMWhere, "KMWhere"},
                {lexer::TokenType::KUndefined, "KUndefined"}
        };
//        return std::pair<int, int>(0, 0);
        return fmt::format("{} {}",tokentype_map.at(token.getType()), token.getValue());
    }
}
