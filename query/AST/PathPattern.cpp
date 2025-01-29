#include "PathPattern.h"

#include "ASTContext.h"

using namespace db;

// EntityPattern
EntityPattern::EntityPattern(VarExpr* var,
                             TypeConstraint* typeConstr,
                             NameConstraint* nameConstr,
                             ExprConstraint* exprConstr)
    : _var(var),
    _typeConstr(typeConstr),
    _nameConstr(nameConstr),
    _exprConstr(exprConstr)
{
}

EntityPattern::~EntityPattern() {
}

EntityPattern* EntityPattern::create(ASTContext* ctxt,
                                     VarExpr* var,
                                     TypeConstraint* typeConstr,
                                     NameConstraint* nameConstr,
                                     ExprConstraint* exprConstr) {
    EntityPattern* pattern = new EntityPattern(var,
                                               typeConstr,
                                               nameConstr,
                                               exprConstr);
    ctxt->addEntityPattern(pattern);
    return pattern;
}

// PathPattern
PathPattern::PathPattern()
{
}

PathPattern::~PathPattern() {
}

PathPattern* PathPattern::create(ASTContext* ctxt) {
    PathPattern* pattern = new PathPattern();
    ctxt->addPathPattern(pattern);
    return pattern;
}

void PathPattern::addElement(EntityPattern* pattern) {
    _elements.push_back(pattern);
}
