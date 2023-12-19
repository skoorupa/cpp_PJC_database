#include "ast.hpp"
#include "parser.hpp"

namespace ast {
    auto nodetype_map = std::map<ast::NodeType, std::string>{
            {ast::NodeType::Program,"Program"},
            {ast::NodeType::NumericLiteral,"NumericLiteral"},
            {ast::NodeType::StringLiteral,"StringLiteral"},
            {ast::NodeType::Identifier,"Identifier"},
            {ast::NodeType::BinaryExpression, "BinaryExpression"},

            {ast::NodeType::Quit,             "Quit"},
            {ast::NodeType::DBConnect,        "DBConnect"},
            {ast::NodeType::DBCreate,         "DBCreate"},

            {ast::NodeType::KFCreateTable,    "KFCreateTable"},
            {ast::NodeType::KFGetTable,       "KFGetTable"},
            {ast::NodeType::KMAddColumn,      "KMAddColumn"},
            {ast::NodeType::KMGetColumn,      "KMGetColumn"},
            {ast::NodeType::KMRename,         "KMRename"},
            {ast::NodeType::KMRemove,         "KMRemove"},
            {ast::NodeType::KMAddRow,         "KMAddRow"},
            {ast::NodeType::KMPrint,          "KMPrint"},

            {ast::NodeType::KMSelect,         "KMSelect"},
            {ast::NodeType::KMWhere,          "KMWhere"}
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
    BinaryExpression::BinaryExpression
    (const Expression &left, const Expression &right, const std::string &expOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), exp_operator(expOperator) {}

            // IDENTIFIER
    Identifier::Identifier(const std::string &symbol) : Expression(NodeType::Identifier), symbol(symbol) {}
    const std::string &Identifier::getSymbol() const {return symbol;}

    // NUMERIC LITERAL
    NumericLiteral::NumericLiteral(int const& value) : Expression(NodeType::NumericLiteral), value(value) {}
    int NumericLiteral::getValue() const {return value;}

    // STRING LITERAL
    StringLiteral::StringLiteral(const std::string& value) : Expression(NodeType::StringLiteral), value(value) {}
    std::string StringLiteral::getValue() const {return value;}

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

    KMAddColumn::KMAddColumn(const std::string &name, const std::string &type)
    : Node(NodeType::KMAddColumn),  name(name), type(type) {}
    const std::string &KMAddColumn::getName() const {return name;}
    const std::string &KMAddColumn::getType() const {return type;}

    KMGetColumn::KMGetColumn(const StringLiteral &column_name) : Node(NodeType::KMGetColumn), column_name(column_name) {}
    const StringLiteral &KMGetColumn::getColumnName() const {return column_name;}

    KMRename::KMRename(const StringLiteral &newName)
            : Node(NodeType::KMRename), new_name(newName) {}
    const StringLiteral &KMRename::getNewName() const {return new_name;}

    KMRemove::KMRemove() : Node(NodeType::KMRemove) {}

    KMAddRow::KMAddRow(const std::vector<db::Value> &values) : Node(NodeType::KMAddRow), values(values) {}
    const std::vector<db::Value> &KMAddRow::getValues() const {return values;}

    KMPrint::KMPrint() : Node(NodeType::KMPrint) {}

    KMSelect::KMSelect(const std::vector<Expression> &args) : Node(NodeType::KMSelect), args(args) {}

    KMWhere::KMWhere(const Expression &expression) : Node(NodeType::KMWhere), expression(expression) {}
}