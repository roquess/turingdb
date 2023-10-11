#include "Expr.h"

#include "ASTContext.h"

using namespace db;

Expr::~Expr() {
}

// VarExpr
VarExpr::VarExpr(const std::string& varName)
    : _varName(varName)
{
}

VarExpr::~VarExpr() {
}

VarExpr* VarExpr::create(ASTContext* ctxt, const std::string& varName) {
    VarExpr* expr = new VarExpr(varName);
    ctxt->addExpr(expr);
    return expr;
}
