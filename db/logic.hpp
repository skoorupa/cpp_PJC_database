#pragma once

#include <memory>
#include "column.hpp"

namespace db {
    enum class Operator {
        Less, LessEqual, More, MoreEqual, Equal, NotEqual, And, Or // LOGIC
    };

    class BinaryExpression : Value {
        std::unique_ptr<Value> left;
        std::unique_ptr<Value> right;
        Operator exp_operator;

    public:
        BinaryExpression(ColumnType type);
    };

    class LogicTranslator {

    };

}
