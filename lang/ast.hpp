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

        Quit,
        DBConnect,
        DBCreate,

        KFCreateTable,
        KFGetTable,
        KMAddColumn,
        KMAddRow,
        KMPrint,

        KMSelect,
        KMWhere
    };

    auto format_as(NodeType nodeType) -> std::string;

    class Node {
        NodeType const kind;

    public:
        Node(const NodeType& kind);

        virtual const NodeType getKind() const;
    };

    class Program : public Node {
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
        Identifier(const std::string &symbol);

        const std::string &getSymbol() const;
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

    class Quit : public Node {
    public:
        Quit();
    };

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

        const StringLiteral &getTableName() const;
    };

    class KFCreateTable : public Node {
        StringLiteral table_name;

    public:
        KFCreateTable(const StringLiteral &tableName);

        const StringLiteral &getTableName() const;
    };

    class KMAddColumn : public Node {
        std::string name;
        std::string type;

    public:
        KMAddColumn(const std::string &name, const std::string &type);

        const std::string &getName() const;

        const std::string &getType() const;
    };

    class KMAddRow : public Node {
        std::vector<std::string> values;

    public:
        KMAddRow(const std::vector<std::string> &values);

        const std::vector<std::string> &getValues() const;
    };

    // DQL

    class KMPrint : public Node {
    public:
        KMPrint();
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
