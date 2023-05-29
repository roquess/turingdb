// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <map>
#include <vector>

#include "DBEntity.h"

#include "StringRef.h"
#include "Range.h"

namespace db {

class Edge;
class Network;
class EdgeType;
class NodeType;
class Writeback;

class Node : public DBEntity {
public:
    friend Writeback;
    using EdgeVector = std::vector<Edge*>;
    using Edges = std::map<const EdgeType*, EdgeVector, DBObject::Comparator>;

    NodeType* getType() const { return (NodeType*)DBEntity::getType(); }

    Network* getNetwork() const { return _net; }

private:
    Network* _net {nullptr};
    Edges _inEdges;
    Edges _outEdges;

    Node(DBIndex index, NodeType* type, Network* net);
    ~Node();
    void addInEdge(Edge* edge);
    void addOutEdge(Edge* edge);
};

}
