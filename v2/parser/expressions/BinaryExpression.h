#pragma once

#include <memory>

#include "Expression.h"
#include "Operators.h"
#include "spdlog/fmt/bundled/core.h"

namespace db {

class BinaryExpression : public Expression {
public:
    BinaryExpression()
        : Expression(ExpressionType::Binary) {}

    ~BinaryExpression() override = default;

    BinaryExpression(const BinaryExpression&) = delete;
    BinaryExpression(BinaryExpression&&) = delete;
    BinaryExpression& operator=(const BinaryExpression&) = delete;
    BinaryExpression& operator=(BinaryExpression&&) = delete;

    BinaryOperator getBinaryOperator() const { return _operator; }
    Expression& left() { return *_left; }
    Expression& right() { return *_right; }

    static std::unique_ptr<BinaryExpression> create(Expression* left, BinaryOperator op, Expression* right) {

        fmt::print("BinaryExpression::create()\n");
        return std::unique_ptr<BinaryExpression> {
            new BinaryExpression {left, op, right}
        };
    }

private:
    BinaryExpression(Expression* left, BinaryOperator op, Expression* right)
        : Expression(ExpressionType::Binary),
          _left(left),
          _right(right),
          _operator(op) {}

    Expression* _left {nullptr};
    Expression* _right {nullptr};
    BinaryOperator _operator {};
};

}
