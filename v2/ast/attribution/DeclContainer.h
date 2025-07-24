#pragma once

#include <vector>

#include "attribution/DeclID.h"
#include "attribution/VariableType.h"

namespace db {

class VariableDecl;

class DeclContainer {
public:
    DeclContainer();
    ~DeclContainer();

    DeclContainer(const DeclContainer&) = delete;
    DeclContainer(DeclContainer&&) = delete;
    DeclContainer& operator=(const DeclContainer&) = delete;
    DeclContainer& operator=(DeclContainer&&) = delete;

    VariableDecl& newDecl(VariableType type);
    VariableDecl& newDecl(VariableType type, std::string_view name);

    VariableDecl& getVariable(DeclID id) {
        return *_variables[id.value()];
    }

    const VariableDecl& getVariable(DeclID id) const {
        return *_variables[id.value()];
    }

private:
    std::vector<std::unique_ptr<VariableDecl>> _variables;
    DeclID _nextID {0};
};

}
