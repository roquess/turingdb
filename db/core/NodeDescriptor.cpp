#include "NodeDescriptor.h"

#include "ComponentType.h"

using namespace db;

NodeDescriptor::NodeDescriptor(NodeType* type)
    : _type(type)
{
}

NodeDescriptor::NodeDescriptor(NodeType* type, NodeDescriptor* parent)
    : _type(type),
    _parent(parent)
{
}

NodeDescriptor::~NodeDescriptor() {
}

bool NodeDescriptor::hasComponent(const ComponentType* compType) const {
    const auto compIt = _compMap.find(compType->getName());
    if (compIt == _compMap.end()) {
        return false;
    }

    return compIt->second == compType;
}

NodeDescriptor* NodeDescriptor::getOrCreateChild(ComponentType* compType) {
    const auto childIt = _childrenMap.find(compType);
    if (childIt != _childrenMap.end()) {
        return childIt->second;
    }

    NodeDescriptor* newDesc = new NodeDescriptor(_type, this);
    _childrenMap[compType] = newDesc;
    return newDesc;
}

void NodeDescriptor::addComponent(ComponentType* compType) {
    _compMap[compType->getName()] = compType;
}

void NodeDescriptor::setBaseComponent(ComponentType* compType) {
    _baseComp = compType;
}
