#pragma once
#include <vector>
#include <string>
#include <memory>
#include "lexer.hpp"
#include "../db/column.hpp"

namespace ast {
    enum class NodeType {
        Program,
        NumericLiteral,
        StringLiteral,
        NullLiteral,
        Identifier,
        BinaryExpression,
        LogicalChainExpression,

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
        KMWhere,
        KMUpdate
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

//    class BinaryExpression : public Expression {
//        std::unique_ptr<Expression> left;
//        std::unique_ptr<Expression> right;
//        std::string exp_operator;
//
//    public:
//        BinaryExpression();
//
//        const std::unique_ptr<Expression> &getLeft() const;
//        const std::unique_ptr<Expression> &getRight() const;
//        const std::string &getExpOperator() const;
//    };

    class LogicalChainExpression : public Expression {
        std::vector<lexer::Token> tokens;

    public:
        LogicalChainExpression();
        LogicalChainExpression(const std::vector<lexer::Token> &tokens);
        const std::vector<lexer::Token> &getTokens() const;
        auto add_token(const lexer::Token& token) -> void;
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
        bool nullable;

    public:
        KMAddColumn(const std::string &name, const std::string &type, const bool &nullable);

        const std::string &getName() const;
        const std::string &getType() const;
        bool isNullable() const;
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

    class KMUpdate : public Node {
        StringLiteral column_name;
        db::Value value;

    public:
        KMUpdate(const StringLiteral &columnName, const db::Value &value);

        const StringLiteral &getColumnName() const;
        const db::Value &getValue() const;
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
        LogicalChainExpression expression;

    public:
        KMWhere(const LogicalChainExpression &expression);
        const LogicalChainExpression &getExpression() const;
    };
}
