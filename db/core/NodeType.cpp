// Copyright 2023 Turing Biosystems Ltd.

#include "NodeType.h"

using namespace db;

NodeType::NodeType(DBIndex index, StringRef name)
    : DBEntityType(index, name)
{
}

NodeType::~NodeType() {
}

NodeType::NodeTypeRange NodeType::inEdgeTypes() const {
    return NodeTypeRange(&_inEdgeTypes);
}

NodeType::NodeTypeRange NodeType::outEdgeTypes() const {
    return NodeTypeRange(&_outEdgeTypes);
}
