// napisane z pomocą https://www.youtube.com/watch?v=8VB5TY1sIRo

#pragma once
#include "lexer.hpp"

#include <fmt/core.h>
#include <fmt/format.h>


namespace lexer {
    auto tokentype_map = std::map<lexer::TokenType, std::string>{
            {lexer::TokenType::BracketRoundBegin, "BracketRoundBegin"},
            {lexer::TokenType::BracketRoundEnd,   "BracketRoundEnd"},
            {lexer::TokenType::DotOperator,       "DotOperator"},
            {lexer::TokenType::CommaOperator,     "CommaOperator"},
            {lexer::TokenType::AsteriskOperator,  "AsteriskOperator"},
            {lexer::TokenType::Number,            "Number"},
            {lexer::TokenType::String,            "String"},
            {lexer::TokenType::Quit,              "Quit"},
            {lexer::TokenType::DBConnect,         "DBConnect"},
            {lexer::TokenType::DBCreate,          "DBCreate"},
            {lexer::TokenType::KFGetTable,        "KFGetTable"},
            {lexer::TokenType::KFCreateTable,     "KFCreateTable"},
            {lexer::TokenType::KMSelect,          "KMSelect"},
            {lexer::TokenType::KMAddColumn,       "KMAddColumn"},
            {lexer::TokenType::KMWhere,           "KMWhere"},
            {lexer::TokenType::Identifier,        "Identifier"},
            {lexer::TokenType::EndOfFile,         "EndOfFile"}
    };

    auto tokenize(std::string input) -> std::deque<Token> {
//        fmt::println("starting to tokenize");
        // HELPFUL VARIABLES
        auto one_char_tokens = std::map<char, lexer::TokenType>{
                {'(',TokenType::BracketRoundBegin},
                {')',TokenType::BracketRoundEnd},
                {'.',TokenType::DotOperator},
                {',',TokenType::CommaOperator},
                {'*',TokenType::AsteriskOperator}
        };
        auto keywords = std::map<std::string, lexer::TokenType>{
                {"quit", TokenType::Quit},
                {"db_connect", TokenType::DBConnect},
                {"db_create", TokenType::DBCreate},
                {"get_table", TokenType::KFGetTable},
                {"create_table", TokenType::KFCreateTable},
                {"add_column", TokenType::KMAddColumn},
                {"select", TokenType::KMSelect},
                {"where", TokenType::KMWhere}
        };
        auto skip = std::set<char>{' ','\n','\t',';'};
        auto isskip = [skip](char c) {
            return skip.contains(c);
        };

        // TOKENIZER
        auto tokens = std::deque<Token>();

        auto pop_front_str = [](std::string& input) {
            input.erase(0,1);
        };

        while (!input.empty()) {
            auto c = input.begin();

            if (one_char_tokens.contains(*c)) {
                tokens.push_back(Token(one_char_tokens.at(*c), std::string(1,*c)));
                pop_front_str(input);
            } else {
                // handle multicharacter tokens

                // string token
                if (*c=='"' || *c=='\'') {
                    auto string_char = *c;
                    auto backslash_flag = false;
                    auto str = std::string();

                    pop_front_str(input); // remove first "

                     do {
//                        fmt::println("{}",*c);
                        str += *c;

                        if (*c == '\\' && !backslash_flag) // \" nie oznacza zamkniecia stringa
                            backslash_flag = true;
                        else
                            backslash_flag = false;

                        pop_front_str(input);
                    } while(!input.empty() && (*c!=string_char || backslash_flag));

                    pop_front_str(input); // remove last "

                    tokens.push_back(Token(TokenType::String, str));
                }
                // number token
                else if (isdigit(*c)) {
                    auto num = std::string();
                    while (!input.empty() && isdigit(*c)) {
                        num += *c;
                        pop_front_str(input);
                    }

                    tokens.push_back(Token(TokenType::Number, num));
                }
                // keyword token
                else if (isalpha(*c) || *c == '_') {
                    auto txt = std::string();
                    while (!input.empty() && (isalpha(*c) || *c == '_')) {
                        txt += *c;
                        pop_front_str(input);
                    }

                    // look for keywords
                    auto iskeyword = keywords.contains(txt);

                    if (iskeyword) {
                        tokens.push_back(Token(keywords.at(txt), txt));
                    } else {
                        tokens.push_back(Token(TokenType::Identifier, txt));
                    }
                } else if (isskip(*c)) {
                    pop_front_str(input);
                } else {
                    fmt::println("!!! Lexer error: cannot recognize character: {}",*c);
                    return tokens;
                }
            }
        }

//        fmt::println("stopped tokenizing");
        tokens.push_back(Token(TokenType::EndOfFile,"EndOfFile"));
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
//        return std::pair<int, int>(0, 0);
        return fmt::format("{} {}",tokentype_map.at(token.getType()), token.getValue());
    }

    auto format_as(TokenType tokentype) -> std::string {
//        return std::pair<int, int>(0, 0);
        return fmt::format("{}",tokentype_map.at(tokentype));
    }
}
