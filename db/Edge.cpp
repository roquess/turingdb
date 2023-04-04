// Copyright 2023 Turing Biosystems Ltd.

#include "Edge.h"

using namespace db;

Edge::Edge(Component* source, Component* target)
    : _source(source),
    _target(target)
{
}

Edge::~Edge() {
}
