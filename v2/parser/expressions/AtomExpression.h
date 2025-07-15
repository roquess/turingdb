#pragma once

#include <memory>
#include <variant>

#include "Literal.h"
#include "Parameter.h"
#include "Symbol.h"
#include "Expression.h"

namespace db {

template <typename T>
concept IsAtomExpression = std::is_same_v<T, Symbol>
                        || std::is_same_v<T, Literal>
                        || std::is_same_v<T, Parameter>;

class AtomExpression : public Expression {
public:
    using ValueType = std::variant<Symbol, Literal, Parameter>;

    AtomExpression() = default;
    ~AtomExpression() override = default;

    AtomExpression(const AtomExpression&) = delete;
    AtomExpression(AtomExpression&&) = delete;
    AtomExpression& operator=(const AtomExpression&) = delete;
    AtomExpression& operator=(AtomExpression&&) = delete;

    static std::unique_ptr<Expression> create(ValueType&& v) {
        return std::unique_ptr<Expression> {new AtomExpression {std::move(v)}};
    }

private:
    explicit AtomExpression(ValueType&& symbol)
        : _value(std::move(symbol)) {
    }

    ValueType _value;
};

}
