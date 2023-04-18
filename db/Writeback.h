#pragma once

#include "StringRef.h"

namespace db {

class DB;
class Network;
class Node;
class NodeType;
class ComponentType;
class ValueType;
class Property;

class Writeback {
public:
    Writeback(DB* db);
    ~Writeback();

    Network* createNetwork(StringRef name);

    Node* createNode(Network* net, NodeType* type);
    bool addComponent(Node* node, ComponentType* compType);

    NodeType* createNodeType(StringRef name);
    ComponentType* createComponentType(StringRef name);

    Property* addProperty(ComponentType* compType,
                          StringRef name,
                          ValueType* valType);

private:
    DB* _db {nullptr};
};

}
