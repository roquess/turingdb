#pragma once

#include <unordered_map>

#include "StringRef.h"

namespace db {

class NodeType;
class ComponentType;
class Writeback;

class NodeDescriptor {
public:
    friend NodeType;
    friend Writeback;

    NodeType* getType() const { return _type; }

    NodeDescriptor* getParent() const { return _parent; }

    ComponentType* getBaseComponent() const { return _baseComp; }

    bool hasComponent(const ComponentType* compType) const;

private:
    NodeType* _type {nullptr};
    NodeDescriptor* _parent {nullptr};
    ComponentType* _baseComp {nullptr};
    std::unordered_map<StringRef, ComponentType*> _compMap;
    std::unordered_map<ComponentType*, NodeDescriptor*> _childrenMap;

    NodeDescriptor(NodeType* type);
    NodeDescriptor(NodeType* type, NodeDescriptor* parent);
    ~NodeDescriptor();

    NodeDescriptor* getOrCreateChild(ComponentType* compType);
    void setBaseComponent(ComponentType* compType);
    void addComponent(ComponentType* compType);
};

}
