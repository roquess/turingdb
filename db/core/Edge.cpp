// Copyright 2023 Turing Biosystems Ltd.

#include "Edge.h"

using namespace db;

Edge::Edge(DBIndex index, EdgeType* type, Node* source, Node* target)
    : DBEntity(index),
    _type(type),
    _source(source),
    _target(target)
{
}

Edge::~Edge() {
}
