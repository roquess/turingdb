#include "Edge.h"

using namespace db;

Edge::Edge(EntityID id, Node* source, Node* target, LabelID label)
    : _id(id),
    _label(label),
    _source(source),
    _target(target)
{
}

Edge::~Edge() {
}