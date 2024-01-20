#include "logic.hpp"
#include <fmt/core.h>
#include <fmt/format.h>

namespace db {
    // OPERATOR
    auto operator_map = std::map<std::string, Operator>{
            {"<",  Operator::Less},
            {"<=", Operator::LessEqual},
            {">",  Operator::More},
            {">=", Operator::MoreEqual},
            {"==", Operator::Equal},
            {"!=", Operator::NotEqual},
            {"&&", Operator::And},
            {"||", Operator::Or}
    };
    auto toOperator(const std::string& s) -> Operator {
        return operator_map.at(s);
    }
    auto format_as(Operator anOperator) -> std::string {
        for (const auto& pair : operator_map)
            if (pair.second == anOperator)
                return pair.first;
        return "unknown";
    }

    auto token_column_type = std::map<lexer::TokenType, db::ColumnType>{
            {lexer::TokenType::Number, db::ColumnType::Integer},
            {lexer::TokenType::String, db::ColumnType::String},
            {lexer::TokenType::Null, db::ColumnType::Null},
            {lexer::TokenType::Identifier, db::ColumnType::Identifier}
    };

    auto pick_column_type(const lexer::Token& token) -> ColumnType {
        if (token_column_type.find(token.getType()) != token_column_type.end())
            return token_column_type.at(token.getType());
        else throw fmt::format("<!!! cannot find tokentype {}",token.getType());
    }

    // BINARYEXPRESSION

    BinaryExpression::BinaryExpression(Value left, Operator expOperator, Value right)
    : Value("", ColumnType::Boolean), left(left), right(right), exp_operator(expOperator) {}

    const Value &BinaryExpression::getLeft() const {return left;}
    const Value &BinaryExpression::getRight() const {return right;}
    Operator BinaryExpression::getExpOperator() const {return exp_operator;}
    void BinaryExpression::setLeft(const Value &left) {BinaryExpression::left = left;}
    void BinaryExpression::setRight(const Value &right) {BinaryExpression::right = right;}
    void BinaryExpression::setExpOperator(Operator expOperator) {exp_operator = expOperator;}

    auto BinaryExpression::evaluate() -> bool {
        if (left.getType() == ColumnType::Null || right.getType() == ColumnType::Null) {
            switch (exp_operator) {
                case Operator::Equal: {
                    return left.getValue() == right.getValue() && left.getType() == right.getType();
                }
                case Operator::NotEqual: {
                    return left.getValue() != right.getValue() && left.getType() != right.getType();
                }
            }
        }
        if (left.getType() != right.getType()) return false;
        switch (exp_operator) {
            case Operator::Equal: {
                return left.getValue() == right.getValue();
            }
            case Operator::NotEqual: {
                return left.getValue() != right.getValue();
            }
        }
        if (left.getType() == ColumnType::Integer) {
            auto l = std::stoi(left.getValue());
            auto r = std::stoi(right.getValue());
            switch (exp_operator) {
                case Operator::Less: {
                    return l < r;
                }
                case Operator::LessEqual: {
                    return l <= r;
                }
                case Operator::More: {
                    return l > r;
                }
                case Operator::MoreEqual: {
                    return l >= r;
                }
            }
        }
        return false;
    }

    // LOGICPARSER

    LogicParser::LogicParser(std::vector<lexer::Token> tokensarg)
    : tokens(tokensarg), point(tokens.begin()) {}

    auto LogicParser::get_token() -> lexer::Token {
        return *get_point();
    };
    auto LogicParser::get_prev_token() -> lexer::Token {
        return *get_point(-1);
    }
    auto LogicParser::consume_token() -> lexer::Token {
        auto token = get_token();
        point++;
        return token;
    }

    auto LogicParser::produce_logic_AST() -> BinaryExpression {
        if (tokens.size() != 3)
            throw std::string("<!!! where condition needs to be binary expression");
        auto left_token = consume_token();
        auto left = Value(left_token.getValue(), pick_column_type(left_token));
        auto exp_operator = toOperator(consume_token().getValue());
        auto right_token = consume_token();
        auto right = Value(right_token.getValue(), pick_column_type(right_token));

        return {left, exp_operator, right};
    }
}