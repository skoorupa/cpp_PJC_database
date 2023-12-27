#pragma once

#include <memory>
#include "../lang/lexer.hpp"
#include "../lang/ast.hpp"
#include "column.hpp"
#include <fmt/format.h>

namespace db {
    enum class Operator {
        Less, LessEqual, More, MoreEqual, Equal, NotEqual, And, Or // LOGIC
    };

    auto pick_column_type(const lexer::Token& token) -> ColumnType;
    auto toOperator(const std::string& s) -> Operator;
    auto format_as(Operator anOperator) -> std::string;

    class BinaryExpression : Value {
        Value left; // zrobmy na razie basic binary expression
        Value right;
        Operator exp_operator;

    public:
        BinaryExpression(Value left, Operator expOperator, Value right);
        const Value &getLeft() const;
        const Value &getRight() const;
        Operator getExpOperator() const;
        void setLeft(const Value &left);
        void setRight(const Value &right);
        void setExpOperator(Operator expOperator);
        auto evaluate() -> bool;
    };

    class LogicParser {
        std::vector<lexer::Token> tokens;
        std::vector<lexer::Token>::iterator point;

        auto get_point (int i = 0) {
            return point+i;
        }
        auto get_token() -> lexer::Token;
        auto get_prev_token() -> lexer::Token;
        auto consume_token() -> lexer::Token;

    public:
        LogicParser(std::vector<lexer::Token> tokens);
        auto produce_logic_AST() -> BinaryExpression;
    };
}
