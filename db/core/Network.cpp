// Copyright 2023 Turing Biosystems Ltd.

#include "Network.h"

using namespace db;

Network::Network(DBIndex index, StringRef name)
    : DBObject(index),
    _name(name)
{
}

Network::~Network() {
}

Network::NodeRange Network::nodes() const {
    return NodeRange(&_nodes);
}

Network::EdgeRange Network::edges() const {
    return EdgeRange(&_edges);
}

void Network::addNode(Node* node) {
    _nodes.push_back(node);
}

void Network::addEdge(Edge* edge) {
    _edges.push_back(edge);
}
