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

        virtual const NodeType getKind() const;
    };

    class Program : public Node {
        NodeType kind;
        std::deque<std::unique_ptr<Node>> body;
    public:
        Program();

//        template<typename T>
//        auto add_node(auto node) -> void {
//            body.emplace_back(std::make_unique<T>(node));
//        };

        std::deque<std::unique_ptr<Node>> &getBody();
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

        std::string getValue() const;
    };

    auto get_expression_value(StringLiteral s) -> std::string;

    //////// NODES

    class DBCreate : public Node {
        StringLiteral db_name;

    public:
        DBCreate(const StringLiteral &stringLiteral);

        const StringLiteral getDbName() const;
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
