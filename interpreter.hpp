#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

namespace interpreter {
    enum class TokenType {
        Function, Method,
        ClosureBegin, ClosureEnd, Separator,
        Expression
    };

    class Token {
        TokenType type;
        std::string name;

        std::map<char, char> closures;
        std::set<char> seperators;

        public:

        Token(TokenType type, const std::string &name);

        TokenType getType() const;

        const std::string &getName() const;
    };

    auto interpret_prompt(std::string input) -> std::vector<Token>;
}

