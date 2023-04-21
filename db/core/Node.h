// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <unordered_map>
#include <vector>

#include "DBObject.h"

#include "StringRef.h"
#include "Value.h"

namespace db {

class NodeType;
class NodeDescriptor;
class Property;
class Edge;
class EdgeType;
class Network;
class Writeback;

class Node : public DBObject {
public:
    friend Writeback;

    Network* getNetwork() const { return _net; }

    Value getProperty(const Property* prop) const;

private:
    using EdgeVector = std::vector<Edge*>;

    NodeDescriptor* _desc {nullptr};
    Network* _net {nullptr};
    std::unordered_map<StringRef, Value> _properties;
    std::unordered_map<EdgeType*, EdgeVector> _inEdges;
    std::unordered_map<EdgeType*, EdgeVector> _outEdges;

    Node(DBIndex index, NodeDescriptor* desc, Network* net);
    ~Node();
    void setDescriptor(NodeDescriptor* desc);
    void addInEdge(Edge* edge);
    void addOutEdge(Edge* edge);
};

}
