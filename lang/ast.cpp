#include "ast.hpp"

namespace ast {
    Node::Node(NodeType kind) : kind(kind) {}

    Program::Program() : Node(NodeType::Program) {
        body = std::vector<Node>();
    }

    BinaryExpression::BinaryExpression
    (Expression &left, Expression &right, std::string &expOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), exp_operator(expOperator) {}

    Expression::Expression(NodeType kind) : Node(kind) {}

    Identifier::Identifier(std::string &symbol) : Expression(NodeType::Identifier), symbol(symbol) {}

    NumericLiteral::NumericLiteral(int &value) : Expression(NodeType::NumericLiteral), value(value) {}

    StringLiteral::StringLiteral(std::string &value) : Expression(NodeType::StringLiteral), value(value) {}
}