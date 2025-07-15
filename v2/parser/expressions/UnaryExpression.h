#pragma once

#include <memory>

#include "Operators.h"
#include "Expression.h"
#include "spdlog/fmt/bundled/core.h"

namespace db {

class UnaryExpression : public Expression {
public:
    UnaryExpression()
        : Expression(ExpressionType::Unary) {
    }

    ~UnaryExpression() override = default;

    UnaryExpression(const UnaryExpression&) = delete;
    UnaryExpression(UnaryExpression&&) = delete;
    UnaryExpression& operator=(const UnaryExpression&) = delete;
    UnaryExpression& operator=(UnaryExpression&&) = delete;

    UnaryOperator getUnaryOperator() const { return _operator; }
    Expression& right() { return *_right; }

    static std::unique_ptr<Expression> create(
        UnaryOperator op,
        Expression* right) {

        fmt::print("UnaryExpression::create\n");
        return std::unique_ptr<Expression> {
            new UnaryExpression {op, right}
        };
    }

private:
    UnaryExpression(UnaryOperator op, Expression* right)
        : Expression(ExpressionType::Unary),
          _right(right),
          _operator(op) {}

    Expression* _right = nullptr;
    UnaryOperator _operator {};
};

}
