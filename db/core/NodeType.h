// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include "StringRef.h"

namespace db {

class NodeDescriptor;
class ComponentType;
class Writeback;

class NodeType {
public:
    friend Writeback;

    StringRef getName() const { return _name; }

    ComponentType* getBaseComponent() const;

private:
    StringRef _name;
    NodeDescriptor* _rootDesc {nullptr};

    NodeType(StringRef name);
    ~NodeType();
};

}
