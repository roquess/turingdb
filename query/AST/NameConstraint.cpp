#include "NameConstraint.h"

#include "ASTContext.h"

using namespace db;

NameConstraint::NameConstraint(const std::string& name)
    : _name(name)
{
}

NameConstraint::~NameConstraint() {
}

NameConstraint* NameConstraint::create(ASTContext* ctxt,
                                       const std::string& name) {
    NameConstraint* constr = new NameConstraint(name);
    ctxt->addNameConstraint(constr);
    return constr;
}
