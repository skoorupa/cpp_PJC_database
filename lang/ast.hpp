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
    auto format_as(NodeType nodeType) -> std::string;

    // STATEMENT
    class Statement {
        NodeType kind;

    public:
        Statement(NodeType kind);
        NodeType getKind() const;
    };

    auto format_as(Statement statement) -> std::string;

    // PROGRAM
    class Program : public Statement {
        NodeType kind;
        std::vector<Statement> body;
    public:
        Program();
        const std::vector<Statement> &getBody() const;
        auto add_statement(Statement statement) -> void;
    };

    auto format_as(Program program) -> std::string;

    // EXPRESSION
    class Expression : public Statement {
    public:
        Expression(NodeType kind);
    };

    auto format_as(Expression expression) -> std::string;

    // BINARY EXPRESSION
    class BinaryExpression : public Expression {
        Expression left;
        Expression right;
        std::string exp_operator;

    public:
        BinaryExpression(Expression &left, Expression &right, std::string &expOperator);

        const Expression &getLeft() const;
        const Expression &getRight() const;
        const std::string &getExpOperator() const;
    };

    auto format_as(BinaryExpression binaryExpression) -> std::string;

    // IDENTIFIER
    class Identifier : public Expression {
        std::string symbol;

    public:
        Identifier(std::string symbol);

        const std::string &getSymbol() const;
    };

    auto format_as(Identifier identifier) -> std::string;

    // NUMERICLITERAL
    class NumericLiteral : public Expression {
        int value;

    public:
        NumericLiteral(int value);

        int getValue() const;
    };

    auto format_as(NumericLiteral numericLiteral) -> std::string;

    // STRINGLITERAL
    class StringLiteral : public Expression {
        std::string value;

    public:
        StringLiteral(std::string &value);

        const std::string &getValue() const;
    };

    auto format_as(StringLiteral stringLiteral) -> std::string;
}
