#pragma once

#include <memory>

#include "Expression.h"
#include "Operators.h"
#include "spdlog/fmt/bundled/core.h"

namespace db {

class StringExpression : public Expression {
public:
    ~StringExpression() override = default;

    StringExpression(const StringExpression&) = delete;
    StringExpression(StringExpression&&) = delete;
    StringExpression& operator=(const StringExpression&) = delete;
    StringExpression& operator=(StringExpression&&) = delete;

    StringOperator getStringOperator() const { return _operator; }
    Expression& right() { return *_right; }


    static std::unique_ptr<Expression> create(StringOperator op, Expression* right) {
        fmt::print("StringExpression::create\n");

        return std::unique_ptr<Expression> {
            new StringExpression {op, right}
        };
    }

private:
    StringExpression(StringOperator op, Expression* right)
        : Expression(ExpressionType::String),
          _right(right),
          _operator(op) {}

    Expression* _right = nullptr;
    StringOperator _operator {};
};

}
