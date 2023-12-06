#include "ast.hpp"

namespace ast {
    // NODE
    Node::Node(NodeType kind) : kind(kind) {}

    // PROGRAM
    Program::Program() : Node(NodeType::Program) {
        body = std::deque<Node>();
    }

    auto Program::add_node(Node node) -> void {
        body.push_back(node);
    }

    // EXPRESSION
    Expression::Expression(NodeType kind) : Node(kind) {}

    //////// EXPRESSIONS

    // BINARY EXPRESSION
    BinaryExpression::BinaryExpression
    (Expression &left, Expression &right, std::string &expOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), exp_operator(expOperator) {}

            // IDENTIFIER
    Identifier::Identifier(std::string &symbol) : Expression(NodeType::Identifier), symbol(symbol) {}

    // NUMERIC LITERAL
    NumericLiteral::NumericLiteral(int &value) : Expression(NodeType::NumericLiteral), value(value) {}

    // STRING LITERAL
    StringLiteral::StringLiteral(std::string &value) : Expression(NodeType::StringLiteral), value(value) {}

    //////// NODES

    DBCreate::DBCreate(const StringLiteral &dbName) : Node(NodeType::DBCreate), db_name(dbName) {}

    DBConnect::DBConnect(const StringLiteral &dbName) : Node(NodeType::DBConnect), db_name(dbName) {}

    KFGetTable::KFGetTable(const StringLiteral &tableName) : Node(NodeType::KFGetTable), table_name(tableName) {}

    KMSelect::KMSelect(const std::vector<Expression> &args) : Node(NodeType::KMSelect), args(args) {}

    KMWhere::KMWhere(const Expression &expression) : Node(NodeType::KMWhere), expression(expression) {}
}