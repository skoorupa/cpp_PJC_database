#include <deque>
#include <vector>
#include <string>

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
        NodeType kind;

    public:
        Node(NodeType kind);
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
        Expression(NodeType kind);
    };

    //////// EXPRESSIONS

    class BinaryExpression : public Expression {
        Expression left;
        Expression right;
        std::string exp_operator;

    public:
        BinaryExpression(Expression &left, Expression &right, std::string &expOperator);
    };

    class Identifier : public Expression {
        std::string symbol;

    public:
        Identifier(std::string &symbol);
    };

    class NumericLiteral : public Expression {
        int value;

    public:
        NumericLiteral(int &value);
    };

    class StringLiteral : public Expression {
        std::string value;

    public:
        StringLiteral(std::string &value);
    };

    //////// NODES

    class DBCreate : public Node {
        StringLiteral db_name;

    public:
        DBCreate(const StringLiteral &dbName);
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
