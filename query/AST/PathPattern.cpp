#include "PathPattern.h"

#include "ASTContext.h"

using namespace db;

// EntityPattern
EntityPattern::EntityPattern(VarExpr* var,
                             TypeConstraint* typeConstr,
                             ExprConstraint* exprConstr,
                             InjectedIDs* injectedIDs,
                             uint64_t entityID)
    : _var(var),
    _typeConstr(typeConstr),
    _exprConstr(exprConstr),
    _injectedIDs(injectedIDs),
    _entityID(entityID)
{
}

EntityPattern::~EntityPattern()
{
}

EntityPattern* EntityPattern::create(ASTContext* ctxt,
                                     VarExpr* var,
                                     TypeConstraint* typeConstr,
                                     ExprConstraint* exprConstr,
                                     InjectedIDs* injectedIDs) {
    EntityPattern* pattern = new EntityPattern(var,
                                               typeConstr,
                                               exprConstr,
                                               injectedIDs,
                                               UINT64_MAX);
    ctxt->addEntityPattern(pattern);
    return pattern;
}

EntityPattern* EntityPattern::create(ASTContext* ctxt,
                                     VarExpr* var,
                                     uint64_t entityID) {
    EntityPattern* pattern = new EntityPattern(var,
                                               nullptr,
                                               nullptr,
                                               nullptr,
                                               entityID);
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
