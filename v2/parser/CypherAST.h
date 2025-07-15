#pragma once

#include <memory>
#include <optional>

#include "Scope.h"
#include "Symbol.h"
#include "expressions/Expression.h"

namespace db {

class CypherAST {
public:
    CypherAST() {
        _rootScope = std::make_unique<Scope>();
        _currentScope = _rootScope.get();
    }

    void beginScope() {
        _currentScope = _currentScope->newInnerScope();
    }

    void endScope() {
        _currentScope = _currentScope->getParentScope();
    }

    Scope& currentScope() {
        return *_currentScope;
    }

    void newNodePattern(std::optional<Symbol>&& symbol,
                        std::optional<std::vector<std::string>>&& labels) {
    }

    template <typename T, typename... Args>
        requires std::is_base_of_v<Expression, T>
    Expression* newExpression(Args&&... args) {
        auto& expr = _expression.emplace_back(T::create(std::forward<Args>(args)...));
        return expr.get();
    }

private:
    std::unique_ptr<Scope> _rootScope;
    std::vector<std::unique_ptr<Expression>> _expression;

    Scope* _currentScope = nullptr;
};

}
