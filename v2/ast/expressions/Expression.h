#pragma once

#include "attribution/DeclID.h"

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

    DeclID id() const { return _id; }

    void setID(DeclID id) { _id = id; }

private:
    DeclID _id {-1};
    ExpressionType _type {};
};

inline Expression::~Expression() = default;

}
