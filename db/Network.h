// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_NETWORK_
#define _DB_NETWORK_

#include <vector>

#include "ComponentStorage.h"
#include "StringRef.h"

namespace db {

class DB;
class Node;

class Network {
public:
    friend DB;
    using Networks = std::vector<Network*>;
    using Nodes = std::vector<Node*>;

    StringRef getDisplayName() const { return _displayName; }

    Network* getParent() const { return _parent; }

    const Networks& subNetworks() const { return _subNets; }

    static Network* create(DB* db, StringRef displayName);
    static Network* create(DB* db, Network* parent, StringRef displayName);

private:
    StringRef _displayName;
    Network* _parent {nullptr};
    Networks _subNets;
    ComponentStorage _compStorage;
    Nodes _nodes;

    Network(StringRef displayName);
    ~Network();
    void setParent(Network* parent);
    void addSubNet(Network* subNet);
};

}

#endif
