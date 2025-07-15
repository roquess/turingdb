#pragma once

#include <memory>
#include <vector>

#include "Expression.h"
#include "Symbol.h"

namespace db {

class NodeLabelExpression : public Expression {
public:
    ~NodeLabelExpression() override = default;

    NodeLabelExpression(const NodeLabelExpression&) = delete;
    NodeLabelExpression(NodeLabelExpression&&) = delete;
    NodeLabelExpression& operator=(const NodeLabelExpression&) = delete;
    NodeLabelExpression& operator=(NodeLabelExpression&&) = delete;

    const Symbol& symbol() { return _symbol; }
    const std::vector<std::string>& labels() { return _labels; }

    static std::unique_ptr<Expression> create(Symbol&& symbol, std::vector<std::string>&& labels) {

        return std::unique_ptr<Expression> {
            new NodeLabelExpression {std::move(symbol), std::move(labels)}
        };
    }

private:
    NodeLabelExpression(Symbol&& symbol, std::vector<std::string>&& labels)
        : Expression(ExpressionType::NodeLabel),
          _symbol(std::move(symbol)),
          _labels(std::move(labels)) {
    }

    Symbol _symbol;
    std::vector<std::string> _labels;
};

}
