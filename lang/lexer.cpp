// napisane z pomocą https://www.youtube.com/watch?v=8VB5TY1sIRo

#pragma once
#include "lexer.hpp"

#include <fmt/core.h>
#include <fmt/format.h>


namespace lexer {
    auto tokentype_map = std::map<lexer::TokenType, std::string>{
            {lexer::TokenType::BracketRoundBegin, "lexer::BracketRoundBegin"},
            {lexer::TokenType::BracketRoundEnd,   "lexer::BracketRoundEnd"},
//            {lexer::TokenType::DotOperator,       "lexer::DotOperator"},
            {lexer::TokenType::CommaOperator,     "lexer::CommaOperator"},
            {lexer::TokenType::AsteriskOperator,  "lexer::AsteriskOperator"},

            {lexer::TokenType::Less,              "lexer::Less"},
            {lexer::TokenType::LessEqual,         "lexer::LessEqual"},
            {lexer::TokenType::More,              "lexer::More"},
            {lexer::TokenType::MoreEqual,         "lexer::MoreEqual"},
            {lexer::TokenType::Equal,             "lexer::Equal"},
            {lexer::TokenType::NotEqual,          "lexer::NotEqual"},
            {lexer::TokenType::And,               "lexer::And"},
            {lexer::TokenType::Or,                "lexer::Or"},

            {lexer::TokenType::Number,            "lexer::Number"},
            {lexer::TokenType::String,            "lexer::String"},
            {lexer::TokenType::Null,              "lexer::Null"},

            {lexer::TokenType::Quit,              "lexer::Quit"},

            {lexer::TokenType::DBConnect,         "lexer::DBConnect"},
            {lexer::TokenType::DBCreate,          "lexer::DBCreate"},
            {lexer::TokenType::DBSave,          "lexer::DBSave"},

            {lexer::TokenType::KFGetTable,        "lexer::KFGetTable"},
            {lexer::TokenType::KFCreateTable,     "lexer::KFCreateTable"},
            {lexer::TokenType::KMAddColumn,       "lexer::KMAddColumn"},
            {lexer::TokenType::KMGetColumn,       "lexer::KMGetColumn"},
            {lexer::TokenType::KMRename,          "lexer::KMRename"},
            {lexer::TokenType::KMRemove,          "lexer::KMRemove"},
            {lexer::TokenType::KMAddRow,          "lexer::KMAddRow"},
            {lexer::TokenType::KMUpdate,          "lexer::KMUpdate"},
            {lexer::TokenType::KMPrint,           "lexer::KMPrint"},
            {lexer::TokenType::KMSelect,          "lexer::KMSelect"},
            {lexer::TokenType::KMWhere,           "lexer::KMWhere"},
            {lexer::TokenType::Identifier,        "lexer::Identifier"},
            {lexer::TokenType::EndOfFile,         "lexer::EndOfFile"}
    };

    auto tokenize(std::string input) -> std::vector<Token> {
//        fmt::println("starting to tokenize");
        // HELPFUL VARIABLES
        auto one_char_tokens = std::map<char, lexer::TokenType>{
                {'(',TokenType::BracketRoundBegin},
                {')',TokenType::BracketRoundEnd},
//                {'.',TokenType::DotOperator},
                {',',TokenType::CommaOperator},
                {'*',TokenType::AsteriskOperator}
        };
        auto keywords = std::map<std::string, lexer::TokenType>{
                {"NULL", TokenType::Null},
                {"quit", TokenType::Quit},
                {"exit", TokenType::Quit},
                {"connect_db", TokenType::DBConnect},
                {"create_db", TokenType::DBCreate},
                {"save_db", TokenType::DBSave},
                {"get_table", TokenType::KFGetTable},
                {"from", TokenType::KFGetTable},
                {"create_table", TokenType::KFCreateTable},
                {"info", TokenType::KMInfo},
                {"add_column", TokenType::KMAddColumn},
                {"get_column", TokenType::KMGetColumn},
                {"rename", TokenType::KMRename},
                {"remove", TokenType::KMRemove},
                {"drop", TokenType::KMRemove},
                {"add_row", TokenType::KMAddRow},
                {"insert", TokenType::KMAddRow},
                {"update", TokenType::KMUpdate},
                {"print", TokenType::KMPrint},
                {"select", TokenType::KMSelect},
                {"where", TokenType::KMWhere}
        };
        auto skip = std::set<char>{' ','\n','\t',';'};
        auto isskip = [skip](char c) {
            return skip.contains(c);
        };
        auto multi_operators = std::map<std::string, lexer::TokenType>{
                {"<",  TokenType::Less},
                {"<=", TokenType::LessEqual},
                {">",  TokenType::More},
                {">=", TokenType::MoreEqual},
                {"==", TokenType::Equal},
                {"!=", TokenType::NotEqual},
                {"&&", TokenType::And},
                {"||", TokenType::Or}
        };
        auto identifier_chars = std::set<char>{'_','~','.'};
        auto operator_chars = std::set<char>{'<','=','>','!','&','|'};
        auto is_multi_operator = [operator_chars](char c) {
            return operator_chars.contains(c);
        };

        // TOKENIZER
        auto tokens = std::vector<Token>();

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

                    while(!input.empty() && (*c!=string_char || backslash_flag)) {
//                        fmt::println("{}",*c);
                        str += *c;

                        if (*c == '\\' && !backslash_flag) // \" nie oznacza zamkniecia stringa
                            backslash_flag = true;
                        else
                            backslash_flag = false;

                        pop_front_str(input);
                    }

                    pop_front_str(input); // remove last "

                    tokens.push_back(Token(TokenType::String, str));
                }
                // number token
                else if (isdigit(*c) || *c == '-') {
                    auto num = std::string();
                    do {
                        num += *c;
                        pop_front_str(input);
                    } while (!input.empty() && isdigit(*c));

                    tokens.push_back(Token(TokenType::Number, num));
                }
                // keyword token
                else if (isalpha(*c) || identifier_chars.contains(*c)) {
                    auto txt = std::string();
                    while (!input.empty() && (isalpha(*c) || identifier_chars.contains(*c) || isdigit(*c))) {
                        txt += *c;
                        pop_front_str(input);
                    }

                    // look for keywords
                    auto iskeyword = keywords.contains(txt);

                    if (iskeyword)
                        tokens.push_back(Token(keywords.at(txt), txt));
                    else
                        tokens.push_back(Token(TokenType::Identifier, txt));
                }
                // OPERATORS
                else if (is_multi_operator(*c)) {
                    auto txt = std::string();

                    while (!input.empty() && is_multi_operator(*c)) {
                        txt += *c;
                        pop_front_str(input);
                    }

                    auto isoperator = multi_operators.contains(txt);
                    if (isoperator)
                        tokens.push_back(Token(multi_operators.at(txt), txt));
                    else
                        throw fmt::format("!!! Lexer error: cannot recognize operator: {}",txt);
                }
                // SKIP
                else if (isskip(*c)) {
                    pop_front_str(input);
                } else {
                    throw fmt::format("!!! Lexer error: cannot recognize character: {}",*c);
                }
            }
        }

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
