#include "ast.hpp"
#include "parser.hpp"

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

//    auto get_expression_value(auto expression) -> std::string {
//        return "null";
//    }
//    auto get_expression_value(StringLiteral s) -> std::string {
//        return s.getValue();
//    }

    //////// EXPRESSIONS

    // BINARY EXPRESSION
    BinaryExpression::BinaryExpression
    (Expression &left, Expression &right, std::string &expOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), exp_operator(expOperator) {}

            // IDENTIFIER
    Identifier::Identifier(std::string &symbol) : Expression(NodeType::Identifier), symbol(symbol) {}

    // NUMERIC LITERAL
    NumericLiteral::NumericLiteral(int const& value) : Expression(NodeType::NumericLiteral), value(value) {}

    // STRING LITERAL
    StringLiteral::StringLiteral(std::string value) : Expression(NodeType::StringLiteral), value(value) {}

    const std::string &StringLiteral::getValue() const {
        return value;
    }

    //////// NODES

    DBCreate::DBCreate(StringLiteral expression) : Node(NodeType::DBCreate), db_name(expression.getValue()) {
        fmt::println("{}",db_name.getValue());
    }

    DBConnect::DBConnect(const StringLiteral &dbName) : Node(NodeType::DBConnect), db_name(dbName) {}

    KFGetTable::KFGetTable(const StringLiteral &tableName) : Node(NodeType::KFGetTable), table_name(tableName) {}

    KMSelect::KMSelect(const std::vector<Expression> &args) : Node(NodeType::KMSelect), args(args) {}

    KMWhere::KMWhere(const Expression &expression) : Node(NodeType::KMWhere), expression(expression) {}
}