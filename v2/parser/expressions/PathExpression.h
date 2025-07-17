#pragma once

#include <memory>
#include <vector>

#include "Expression.h"
#include "Symbol.h"

namespace db {

class PathExpression : public Expression {
public:
    PathExpression() = delete;
    ~PathExpression() override = default;

    PathExpression(const PathExpression&) = delete;
    PathExpression(PathExpression&&) = delete;
    PathExpression& operator=(const PathExpression&) = delete;
    PathExpression& operator=(PathExpression&&) = delete;

    PathExpression(Symbol&& symbol, std::vector<std::string>&& labels)
        : Expression(ExpressionType::Path),
          _symbol(std::move(symbol)),
          _labels(std::move(labels)) {
    }

    const Symbol& symbol() { return _symbol; }
    const std::vector<std::string>& labels() { return _labels; }

    static std::unique_ptr<PathExpression> create(Symbol&& symbol,
                                              std::vector<std::string>&& labels) {
        return std::make_unique<PathExpression>(std::move(symbol), std::move(labels));
    }

private:
    Symbol _symbol;
    std::vector<std::string> _labels;
};

}
