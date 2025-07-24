#pragma once

#include "attribution/ASTNodeID.h"

namespace db {

enum class ExpressionType {
    Binary = 0,
    Unary,
    String,
    NodeLabel,
    Property,
    Atom,
    Path,
};

class Expression {
public:
    explicit Expression(ExpressionType type)
        : _type(type)
    {
    }

    Expression() = delete;
    virtual ~Expression() = 0;

    Expression(const Expression&) = delete;
    Expression(Expression&&) = delete;
    Expression& operator=(const Expression&) = delete;
    Expression& operator=(Expression&&) = delete;

    ExpressionType type() const { return _type; }

    template <typename T>
    T* as() {
        return dynamic_cast<T*>(this);
    }

    template <typename T>
    const T* as() const {
        return dynamic_cast<const T*>(this);
    }

    ASTNodeID id() const { return _id; }

    void setID(ASTNodeID id) { _id = id; }

private:
    ASTNodeID _id;
    ExpressionType _type {};
};

inline Expression::~Expression() = default;

}
