#include <vector>
#include <string>

namespace ast {
    enum class NodeType {
        Program,
        NumericLiteral,
        StringLiteral,
        Identifier,
        BinaryExpression
    };

    class Statement {
        NodeType kind;

    public:
        Statement(NodeType kind);
    };

    class Program : public Statement {
        NodeType kind;
        std::vector<Statement> body;
    public:
        Program();
    };

    class Expression : public Statement {
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
