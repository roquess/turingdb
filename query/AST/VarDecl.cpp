#include "VarDecl.h"

#include "ASTContext.h"
#include "DeclContext.h"

using namespace db;

VarDecl::VarDecl(DeclKind kind, const std::string& name, uint64_t entityID)
    : _kind(kind),
    _name(name),
    _entityID(entityID)
{
}

VarDecl::~VarDecl() {
}

VarDecl* VarDecl::create(ASTContext* astCtxt,
                         DeclContext* declContext,
                         const std::string& name,
                         DeclKind kind,
                         uint64_t entityID) {
    if (declContext->getDecl(name)) {
        return nullptr;
    }

    VarDecl* decl = new VarDecl(kind, name, entityID);
    astCtxt->addVarDecl(decl);
    declContext->addDecl(decl);

    return decl;
}
