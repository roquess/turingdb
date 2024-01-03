#pragma once

#include "EntityID.h"
#include "RWSpinLock.h"
#include "LabelID.h"

namespace db {

class StorageAccessor;
class EdgeAccessor;
class Node;

class Edge {
public:
    friend StorageAccessor;
    friend EdgeAccessor;

private:
    EntityID _id;
    bool _deleted {false};
    mutable RWSpinLock _lock;
    LabelID _label;
    Node* _source {nullptr};
    Node* _target {nullptr};

    Edge(EntityID id, Node* source, Node* target, LabelID label);
    ~Edge();
};

}