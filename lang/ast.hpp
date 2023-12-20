#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../db/column.hpp"

namespace ast {
    enum class NodeType {
        Program,
        NumericLiteral,
        StringLiteral,
        NullLiteral,
        Identifier,
        BinaryExpression,

        Quit,
        DBConnect,
        DBCreate,

        KFCreateTable,
        KFGetTable,
        KMAddColumn,
        KMGetColumn,
        KMRename,
        KMRemove,
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
        std::vector<std::unique_ptr<Node>> body;
    public:
        Program();

//        template<typename T>
//        auto add_node(auto node) -> void {
//            body.emplace_back(std::make_unique<T>(node));
//        };

        std::vector<std::unique_ptr<Node>> &getBody();
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

        int getValue() const;
    };

    class StringLiteral : public Expression {
        std::string value;

    public:
        StringLiteral(const std::string& value);

        std::string getValue() const;
        void setValue(const std::string &value);
    };

    class NullLiteral : public Expression {
    public:
        NullLiteral();
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

    class KMGetColumn : public Node {
        StringLiteral column_name;

    public:
        KMGetColumn(const StringLiteral &column_name);

        const StringLiteral &getColumnName() const;
    };

    class KMRename : public Node {
        StringLiteral new_name;

    public:
        KMRename(const StringLiteral &newName);

        const StringLiteral &getNewName() const;
    };

    class KMRemove : public Node {
    public:
        KMRemove();
    };

    class KMAddRow : public Node {
        std::vector<db::Value> values;

    public:
        KMAddRow(const std::vector<db::Value> &values);

        const std::vector<db::Value> &getValues() const;
    };

    // DQL

    class KMPrint : public Node {
    public:
        KMPrint();
    };

    class KMSelect : public Node {
        std::vector<std::unique_ptr<Expression>> expressions;

    public:
        KMSelect();
        KMSelect(std::vector<std::unique_ptr<Expression>> expressions);

        const std::vector<std::unique_ptr<Expression>>& getExpressions() const;
        auto add_expression(std::unique_ptr<ast::Expression>& expression) -> void;
    };

    class KMWhere : public Node {
        Expression expression;

    public:
        KMWhere(const Expression &expression);
    };
}
