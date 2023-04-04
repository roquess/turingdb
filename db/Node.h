// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_NODE_
#define _DB_NODE_

#include <vector>

namespace db {

class NodeType;
class Component;

class Node {
public:
    NodeType* getType() const { return _type; }

private:
    NodeType* _type {nullptr};
    Component* _base {nullptr};
    std::vector<Component*> _components;

    Node(NodeType* type);
    ~Node();
};

}

#endif
