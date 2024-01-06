#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

namespace lexer {
    enum class TokenType {
        BracketRoundBegin,
        BracketRoundEnd,
        DotOperator,
        CommaOperator,
        AsteriskOperator,

        Less,
        LessEqual,
        More,
        MoreEqual,
        Equal,
        NotEqual,
        And,
        Or,

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
        KMRename,
        KMRemove,
        KMAddRow,
        KMUpdate,
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

    auto tokenize(std::string s) -> std::vector<Token>;
    auto format_as (Token token) -> std::string;
    auto format_as (TokenType tokentype) -> std::string;
}

