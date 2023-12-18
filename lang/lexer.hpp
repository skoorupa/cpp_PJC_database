#pragma once
#include <string>
#include <deque>
#include <map>
#include <set>

namespace lexer {
    enum class TokenType {
        BracketRoundBegin,
        BracketRoundEnd,
        DotOperator,
        CommaOperator,
        AsteriskOperator,

        Number,
        String,
        Null,

        Quit,
        DBConnect,
        DBCreate,

        KFCreateTable,
        KFGetTable,
        KMAddColumn,
        KMGetColumn,
        KMRenameColumn,
        KMAddRow,
        KMPrint,
        KMSelect,
        KMWhere,
        Identifier,
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
    auto format_as (TokenType tokentype) -> std::string;
}

