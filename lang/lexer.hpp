#pragma once
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

namespace lexer {
    enum class TokenType {
        Identifier,
        BracketRoundBegin,
        BracketRoundEnd,
        DotOperator,
        AsteriskOperator,

        Number,
        String,

        DBConnect,
        DBCreate,

        KFGetTable,
        KMSelect,
        KMWhere,
        KUndefined,
        EndOfFile
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

    auto tokenize(std::string input) -> std::deque<Token>;
    auto format_as (Token token) -> std::string;
}

