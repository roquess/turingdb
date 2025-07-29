#include "InjectedIDs.h"

#include "ASTContext.h"

using namespace db;

InjectedIDs::InjectedIDs()
{
}

InjectedIDs::~InjectedIDs() {
}

void InjectedIDs::addID(NodeID id) {
    _nodeIDs.push_back(id);
}

InjectedIDs* InjectedIDs::create(ASTContext* ctxt) {
    InjectedIDs* injectedIDs = new InjectedIDs();
    ctxt->addInjectedIDs(injectedIDs);
    return injectedIDs;
}
