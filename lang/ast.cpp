#include "ast.hpp"
#include "parser.hpp"

namespace ast {
    // NODE
    Node::Node(const NodeType& kind) : kind(kind) {}

    const NodeType Node::getKind() const {
        return kind;
    }

    // PROGRAM
    Program::Program() : Node(NodeType::Program) {
        body = std::deque<std::unique_ptr<Node>>();
    }

    std::deque<std::unique_ptr<Node>> &Program::getBody() {
        return body;
    }

    // EXPRESSION
    Expression::Expression(const NodeType& kind) : Node(kind) {}

    //////// EXPRESSIONS

    // BINARY EXPRESSION
    BinaryExpression::BinaryExpression
    (const Expression &left, const Expression &right, const std::string &expOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), exp_operator(expOperator) {}

            // IDENTIFIER
    Identifier::Identifier(std::string &symbol) : Expression(NodeType::Identifier), symbol(symbol) {}

    // NUMERIC LITERAL
    NumericLiteral::NumericLiteral(int const& value) : Expression(NodeType::NumericLiteral), value(value) {}

    // STRING LITERAL
    StringLiteral::StringLiteral(const std::string& value) : Expression(NodeType::StringLiteral), value(value) {}

    std::string StringLiteral::getValue() const {
        return value;
    }

    //////// NODES

    Quit::Quit() : Node(NodeType::Quit) {}

    DBCreate::DBCreate(const StringLiteral& stringLiteral) : Node(NodeType::DBCreate), db_name(stringLiteral) {}

    const StringLiteral DBCreate::getDbName() const {
        return db_name;
    }

    DBConnect::DBConnect(const StringLiteral &dbName) : Node(NodeType::DBConnect), db_name(dbName) {}

    KFGetTable::KFGetTable(const StringLiteral &tableName) : Node(NodeType::KFGetTable), table_name(tableName) {}

    KMSelect::KMSelect(const std::vector<Expression> &args) : Node(NodeType::KMSelect), args(args) {}

    KMWhere::KMWhere(const Expression &expression) : Node(NodeType::KMWhere), expression(expression) {}
}