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
        KMWhere,
        KUndefined,
        EndOfFile
    };

    class Node {
        NodeType kind;

    public:
        Node(NodeType kind);
    };

    class Program : public Node {
        NodeType kind;
        std::vector<Node> body;
    public:
        Program();
    };

    class Expression : public Node {
    public:
        Expression(NodeType kind);
    };

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
}
