// Copyright 2023 Turing Biosystems Ltd.

#include "Network.h"

using namespace db;

Network::Network(StringRef displayName)
    : _displayName(displayName)
{
}

Network::~Network() {
}

void Network::setParent(Network* parent) {
    _parent = parent;
}

void Network::addSubNet(Network* subnet) {
    _subNets.push_back(subnet);
}
