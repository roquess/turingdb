// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <vector>

#include "DBObject.h"
#include "StringRef.h"

namespace db {

class Node;
class Edge;
class NetworkNodeRange;
class NetworkEdgeRange;
class Writeback;

class Network : public DBObject {
public:
    friend NetworkNodeRange;
    friend NetworkEdgeRange;
    friend Writeback;

    StringRef getName() const { return _name; }

private:
    using Nodes = std::vector<Node*>;
    using Edges = std::vector<Edge*>;
    StringRef _name;
    Nodes _nodes;
    Edges _edges;

    Network(DBIndex index, StringRef name);
    ~Network();
    void addNode(Node* node);
    void addEdge(Edge* edge);
};

}
