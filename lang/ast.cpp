#include "ast.hpp"
#include "parser.hpp"

namespace ast {
    auto nodetype_map = std::map<ast::NodeType, std::string>{
            {ast::NodeType::Program,                "ast::Program"},
            {ast::NodeType::NumericLiteral,         "ast::NumericLiteral"},
            {ast::NodeType::StringLiteral,          "ast::StringLiteral"},
            {ast::NodeType::NullLiteral,            "ast::NullLiteral"},
            {ast::NodeType::Identifier,             "ast::Identifier"},
            {ast::NodeType::BinaryExpression,       "ast::BinaryExpression"},
            {ast::NodeType::LogicalChainExpression, "ast::LogicalChainExpression"},

            {ast::NodeType::Quit,                   "ast::Quit"},
            {ast::NodeType::DBConnect,              "ast::DBConnect"},
            {ast::NodeType::DBCreate,               "ast::DBCreate"},

            {ast::NodeType::KFCreateTable,          "ast::KFCreateTable"},
            {ast::NodeType::KFGetTable,             "ast::KFGetTable"},
            {ast::NodeType::KMInfo,                 "ast::KMInfo"},
            {ast::NodeType::KMAddColumn,            "ast::KMAddColumn"},
            {ast::NodeType::KMGetColumn,            "ast::KMGetColumn"},
            {ast::NodeType::KMRename,               "ast::KMRename"},
            {ast::NodeType::KMRemove,               "ast::KMRemove"},
            {ast::NodeType::KMAddRow,               "ast::KMAddRow"},
            {ast::NodeType::KMPrint,                "ast::KMPrint"},

            {ast::NodeType::KMSelect,               "ast::KMSelect"},
            {ast::NodeType::KMWhere,                "ast::KMWhere"},
            {ast::NodeType::KMUpdate,               "ast::KMUpdate"}
    };

    auto format_as(NodeType nodeType) -> std::string {
        return fmt::format("{}",nodetype_map.at(nodeType));
    }

    // NODE
    Node::Node(const NodeType& kind) : kind(kind) {}
    const NodeType Node::getKind() const {return kind;}

    // PROGRAM
    Program::Program() : Node(NodeType::Program), body(std::vector<std::unique_ptr<Node>>()) {}
    std::vector<std::unique_ptr<Node>> &Program::getBody() {return body;}

    // EXPRESSION
    Expression::Expression(const NodeType& kind) : Node(kind) {}

    //////// EXPRESSIONS

    // BINARY EXPRESSION
//    BinaryExpression::BinaryExpression()
//            : Expression(NodeType::BinaryExpression), left(), right(), exp_operator("") {}
//
//    const std::unique_ptr<Expression> &BinaryExpression::getLeft() const {return left;}
//    const std::unique_ptr<Expression> &BinaryExpression::getRight() const {return right;}
//    const std::string &BinaryExpression::getExpOperator() const {return exp_operator;}
    // LOGICAL EXPRESSION
    LogicalChainExpression::LogicalChainExpression()
            : Expression(NodeType::LogicalChainExpression), tokens() {}
    LogicalChainExpression::LogicalChainExpression(const std::vector<lexer::Token> &tokens)
            : Expression(NodeType::LogicalChainExpression), tokens(tokens) {}
    const std::vector<lexer::Token> &LogicalChainExpression::getTokens() const {return tokens;}

    auto LogicalChainExpression::add_token(const lexer::Token& token) -> void {
        tokens.push_back(token);
    }

    // IDENTIFIER
    Identifier::Identifier(const std::string &symbol) : Expression(NodeType::Identifier), symbol(symbol) {}
    const std::string &Identifier::getSymbol() const {return symbol;}

    // NUMERIC LITERAL
    NumericLiteral::NumericLiteral(int const& value) : Expression(NodeType::NumericLiteral), value(value) {}
    int NumericLiteral::getValue() const {return value;}

    // STRING LITERAL
    StringLiteral::StringLiteral(const std::string& value) : Expression(NodeType::StringLiteral), value(value) {}
    std::string StringLiteral::getValue() const {return value;}
    void StringLiteral::setValue(const std::string &value) {StringLiteral::value = value;}

    NullLiteral::NullLiteral() : Expression(NodeType::NullLiteral) {}

    //////// NODES

    Quit::Quit() : Node(NodeType::Quit) {}

    DBCreate::DBCreate(const StringLiteral& stringLiteral) : Node(NodeType::DBCreate), db_name(stringLiteral) {}
    const StringLiteral DBCreate::getDbName() const {return db_name;}

    DBConnect::DBConnect(const StringLiteral &dbName) : Node(NodeType::DBConnect), db_name(dbName) {}

    KFCreateTable::KFCreateTable(const ast::StringLiteral &tableName) : Node(NodeType::KFCreateTable), table_name(tableName) {}
    const StringLiteral &KFCreateTable::getTableName() const {return table_name;}

    KFGetTable::KFGetTable(const StringLiteral &tableName) : Node(NodeType::KFGetTable), table_name(tableName) {}
    const StringLiteral &KFGetTable::getTableName() const {return table_name;}

    KMInfo::KMInfo() : Node(NodeType::KMInfo) {}

    KMAddColumn::KMAddColumn(const std::string &name, const std::string &type, const bool &nullable)
    : Node(NodeType::KMAddColumn),  name(name), type(type), nullable(nullable) {}
    const std::string &KMAddColumn::getName() const {return name;}
    const std::string &KMAddColumn::getType() const {return type;}
    bool KMAddColumn::isNullable() const {return nullable;}

    KMGetColumn::KMGetColumn(const StringLiteral &column_name) : Node(NodeType::KMGetColumn), column_name(column_name) {}
    const StringLiteral &KMGetColumn::getColumnName() const {return column_name;}

    KMRename::KMRename(const StringLiteral &newName)
            : Node(NodeType::KMRename), new_name(newName) {}
    const StringLiteral &KMRename::getNewName() const {return new_name;}

    KMRemove::KMRemove() : Node(NodeType::KMRemove) {}

    KMAddRow::KMAddRow(const std::vector<db::Value> &values) : Node(NodeType::KMAddRow), values(values) {}
    const std::vector<db::Value> &KMAddRow::getValues() const {return values;}

    KMUpdate::KMUpdate(const StringLiteral &columnName, const db::Value &value)
    : Node(NodeType::KMUpdate), column_name(columnName), value(value) {}
    const StringLiteral &KMUpdate::getColumnName() const {return column_name;}
    const db::Value &KMUpdate::getValue() const {return value;}

    KMPrint::KMPrint() : Node(NodeType::KMPrint) {}

    KMSelect::KMSelect() : Node(NodeType::KMSelect), expressions(std::vector<std::unique_ptr<Expression>>()) {}
    KMSelect::KMSelect(std::vector<std::unique_ptr<Expression>> expressions)
    : Node(NodeType::KMSelect), expressions(std::move(expressions)) {}
    const std::vector<std::unique_ptr<Expression>> &KMSelect::getExpressions() const {return expressions;};
    auto KMSelect::add_expression(std::unique_ptr<ast::Expression>& expression) -> void {
        expressions.push_back(std::move(expression));
    }

    KMWhere::KMWhere(const LogicalChainExpression &expression) : Node(NodeType::KMWhere), expression(expression) {}
    const LogicalChainExpression &KMWhere::getExpression() const {return expression;}
}