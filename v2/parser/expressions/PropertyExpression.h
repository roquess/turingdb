#pragma once

#include <memory>

#include "Expression.h"
#include "QualifiedName.h"
#include "spdlog/fmt/bundled/core.h"

namespace db {

class PropertyExpression : public Expression {
public:
    ~PropertyExpression() override = default;

    PropertyExpression(const PropertyExpression&) = delete;
    PropertyExpression(PropertyExpression&&) = delete;
    PropertyExpression& operator=(const PropertyExpression&) = delete;
    PropertyExpression& operator=(PropertyExpression&&) = delete;

    const QualifiedName& name() { return _name; }

    static std::unique_ptr<Expression> create(QualifiedName&& name) {

        fmt::print("PropertyExpression::create\n");
        return std::unique_ptr<Expression> {
            new PropertyExpression {std::move(name)}};
    }

private:
    PropertyExpression(QualifiedName&& name)
        : Expression(ExpressionType::Property),
          _name(std::move(name)) {
    }

    QualifiedName _name;
};

}
