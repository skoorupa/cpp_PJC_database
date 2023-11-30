#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

namespace lexer {
    enum class TokenType {
        Function, Method,
        ClosureBegin, ClosureEnd, Separator,
        Expression,

        BracketRoundBegin,
        BracketRoundEnd,
        Number,

        KFGetTable,
        KMSelect,
        KMWhere,
        KUndefined
    };

    class Token {
        TokenType type;
        std::string value;

        std::map<char, char> closures;
        std::set<char> seperators;
        std::map<std::string, TokenType> keywords;

        public:

        Token(TokenType type, const std::string &value);

        TokenType getType() const;

        const std::string &getValue() const;
    };

    auto tokenize(std::string input) -> std::vector<Token>;
    auto format_as (Token token) -> std::string;
}

