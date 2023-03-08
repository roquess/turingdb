// Copyright 2023 Turing Biosystems Ltd.

#include "Network.h"

#include "DB.h"
#include "Object.h"

using namespace db;

Network::Network(StringRef displayName)
    : _displayName(displayName)
{
}

Network::~Network() {
    for (Network* net : _subNets) {
        delete net;
    }

    for (Object* obj : _objects) {
        delete obj;
    }
}

Network* Network::create(DB* db, StringRef displayName) {
    Network* net = new Network(displayName);
    db->addNetwork(net);
    return net;
}

Network* Network::create(DB* db, Network* parent, StringRef displayName) {
    Network* net = create(db, displayName);
    net->setParent(parent);
    parent->addSubNet(net);
    return net;
}

void Network::setParent(Network* parent) {
    _parent = parent;
}

void Network::addSubNet(Network* subNet) {
    _subNets.push_back(subNet);
}

void Network::addObject(Object* obj) {
    _objects.push_back(obj);
}

void Network::addInternalLink(Link* link) {
    _internalLinks.push_back(link);
}

void Network::addInLink(Link* link) {
    _inLinks.push_back(link);
}

void Network::addOutLink(Link* link) {
    _outLinks.push_back(link);
}
