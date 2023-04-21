// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <vector>

#include "StringRef.h"
#include "DBIndex.h"

namespace db {

class Node;
class Edge;
class NetworkNodeRange;
class NetworkEdgeRange;
class Writeback;

class Network {
public:
    friend NetworkNodeRange;
    friend NetworkEdgeRange;
    friend Writeback;
    using Nodes = std::vector<Node*>;
    using Edges = std::vector<Edge*>;

    StringRef getName() const { return _name; }

private:
    DBIndex::ID _id;
    StringRef _name;
    Nodes _nodes;
    Edges _edges;
    DBIndex::ID _nextFreeNodeID {0};

    Network(DBIndex::ID id, StringRef name);
    ~Network();
    DBIndex allocNodeIndex();
    void addNode(Node* node);
    void addEdge(Edge* edge);
};

}
