// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <map>
#include <vector>

#include "DBEntity.h"

#include "StringRef.h"

namespace db {

class Edge;
class Network;
class EdgeType;
class NodeType;
class NodeEdgeRange;
class Writeback;

class Node : public DBEntity {
public:
    friend NodeEdgeRange;
    friend Writeback;

    NodeType* getType() const { return (NodeType*)DBEntity::getType(); }

    Network* getNetwork() const { return _net; }

private:
    using EdgeVector = std::vector<Edge*>;
    using Edges = std::map<const EdgeType*, EdgeVector, DBObject::Comparator>;

    Network* _net {nullptr};
    Edges _inEdges;
    Edges _outEdges;

    Node(DBIndex index, NodeType* type, Network* net);
    ~Node();
    void addInEdge(Edge* edge);
    void addOutEdge(Edge* edge);
};

}
