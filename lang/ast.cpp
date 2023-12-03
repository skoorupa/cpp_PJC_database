#include "ast.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <map>

namespace ast {
    // STATEMENT
    Statement::Statement(NodeType kind) : kind(kind) {}

    NodeType Statement::getKind() const {
        return kind;
    }

    // PROGRAM
    Program::Program() : Statement(NodeType::Program) {
        body = std::vector<Statement>();
    }

    auto Program::add_statement(ast::Statement statement) -> void {
        body.push_back(statement);
    }

    const std::vector<Statement> &Program::getBody() const {
        return body;
    }

    // EXPRESSION
    Expression::Expression(NodeType kind) : Statement(kind) {}

    // BINARY EXPRESSION
    BinaryExpression::BinaryExpression
    (Expression &left, Expression &right, std::string &expOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), exp_operator(expOperator) {}

    const Expression &BinaryExpression::getLeft() const {return left;}
    const Expression &BinaryExpression::getRight() const {return right;}
    const std::string &BinaryExpression::getExpOperator() const {return exp_operator;}

    // IDENTIFIER
    Identifier::Identifier(std::string symbol) : Expression(NodeType::Identifier), symbol(symbol) {}

    const std::string &Identifier::getSymbol() const {return symbol;}

    // NUMERICLITERAL
    NumericLiteral::NumericLiteral(int value) : Expression(NodeType::NumericLiteral), value(value) {}

    int NumericLiteral::getValue() const {return value;}

    // STRINGLITERAL
    StringLiteral::StringLiteral(std::string &value) : Expression(NodeType::StringLiteral), value(value) {}

    const std::string &StringLiteral::getValue() const {return value;}

    // // FORMATAS
    auto format_as(NodeType nodeType) -> std::string {
        auto map = std::map<NodeType, std::string>{
                {NodeType::Program, "Program"},
                {NodeType::NumericLiteral, "NumericLiteral"},
                {NodeType::StringLiteral, "StringLiteral"},
                {NodeType::Identifier, "Identifier"},
                {NodeType::BinaryExpression, "BinaryExpression"}
        };
        return map.at(nodeType);
    }

    auto format_as(Statement statement) -> std::string {
        return fmt::format("Statement({})", statement.getKind());
    }

    auto format_as(Program program) -> std::string {
        return fmt::format(
                "Program(kind: {}, body : {})",
                format_as(program.getKind()),
                program.getBody()
                );
    }

    auto format_as(Expression expression) -> std::string {
        return fmt::format("Expression({})", expression);
    }

    auto format_as(BinaryExpression binaryExpression) -> std::string {
        return fmt::format(
                "BinaryExpression({} {} {})",
                binaryExpression.getLeft(),
                binaryExpression.getExpOperator(),
                binaryExpression.getRight()
                );
    }

    auto format_as(Identifier identifier) -> std::string {
        return fmt::format("Identifier({})", identifier.getSymbol());
    }

    auto format_as(NumericLiteral numericLiteral) -> std::string {
        return fmt::format("NumericLiteral({})", numericLiteral.getValue());
    }

    auto format_as(StringLiteral stringLiteral) -> std::string {
        return fmt::format("StringLiteral({})", stringLiteral.getValue());
    }
}