// Copyright 2023 Turing Biosystems Ltd.

#include "Network.h"

using namespace db;

Network::Network(DBIndex::ID id, StringRef name)
    : _id(id),
    _name(name)
{
}

Network::~Network() {
}

DBIndex Network::allocNodeIndex() {
    const DBIndex::ID nodeID = _nextFreeNodeID;
    _nextFreeNodeID++;

    return DBIndex(_id, nodeID);
}

void Network::addNode(Node* node) {
    _nodes.push_back(node);
}

void Network::addEdge(Edge* edge) {
    _edges.push_back(edge);
}
