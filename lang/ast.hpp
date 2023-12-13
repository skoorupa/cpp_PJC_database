#pragma once
#include <deque>
#include <vector>
#include <string>
#include <memory>

namespace ast {
    enum class NodeType {
        Program,
        NumericLiteral,
        StringLiteral,
        Identifier,
        BinaryExpression,

        DBConnect,
        DBCreate,

        KFGetTable,
        KMSelect,
        KMWhere
    };

    class Node {
        NodeType const kind;

    public:
        Node(const NodeType& kind);
    };

    class Program : public Node {
        NodeType kind;
        std::deque<Node> body;
    public:
        Program();
        auto add_node(Node node) -> void;
    };

    class Expression : public Node {
    public:
        Expression(const NodeType& kind);
    };

    //////// EXPRESSIONS

    class BinaryExpression : public Expression {
        Expression left;
        Expression right;
        std::string exp_operator;

    public:
        BinaryExpression(const Expression &left, const Expression &right, const std::string &expOperator);
    };

    class Identifier : public Expression {
        std::string symbol;

    public:
        Identifier(std::string &symbol);
    };

    class NumericLiteral : public Expression {
        int value;

    public:
        NumericLiteral(int const& value);
    };

    class StringLiteral : public Expression {
        const std::string value;

    public:
        StringLiteral(const std::string& value);

        std::string getValue();
    };

    auto get_expression_value(StringLiteral s) -> std::string;

    //////// NODES

    class DBCreate : public Node {
        StringLiteral db_name;

    public:
        DBCreate(const StringLiteral &stringLiteral);
    };

    class DBConnect : public Node {
        StringLiteral db_name;

    public:
        DBConnect(const StringLiteral &dbName);
    };

    class KFGetTable : public Node {
        StringLiteral table_name;

    public:
        KFGetTable(const StringLiteral &tableName);
    };

    class KMSelect : public Node {
        std::vector<Expression> args;

    public:
        KMSelect(const std::vector<Expression> &args);
    };

    class KMWhere : public Node {
        Expression expression;

    public:
        KMWhere(const Expression &expression);
    };
}
