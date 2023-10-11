#include "FromTarget.h"

#include "ASTContext.h"

using namespace db;

FromTarget::FromTarget()
{
}

FromTarget::~FromTarget() {
}

void FromTarget::addPathPattern(PathPattern* pattern) {
    _patterns.push_back(pattern);
}

FromTarget* FromTarget::create(ASTContext* ctxt) {
    FromTarget* target = new FromTarget();
    ctxt->addFromTarget(target);
    return target;
}
