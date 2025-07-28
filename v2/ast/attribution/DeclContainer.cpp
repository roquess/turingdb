#include "attribution/DeclContainer.h"
#include "attribution/VarDecl.h"

using namespace db;

DeclContainer::DeclContainer() = default;

DeclContainer::~DeclContainer() = default;

VarDecl& DeclContainer::newDecl(EvaluatedType type) {
    const auto id = _nextID++;
    auto& decl = _decls.emplace_back(std::make_unique<VarDecl>(type, id));

    return *decl;
}

VarDecl& DeclContainer::newDecl(EvaluatedType type, std::string_view name) {
    const auto id = _nextID++;
    auto& decl = _decls.emplace_back(std::make_unique<VarDecl>(type, id));
    decl->setName(name);

    return *decl;
}
