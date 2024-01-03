#pragma once

#include <map>
#include <mutex>
#include <shared_mutex>
#include <atomic>

#include "EntityID.h"
#include "StorageAccessor.h"
#include "RWSpinLock.h"
#include "LabelIndex.h"

namespace db {

class Node;
class Edge;
class NodeAccessor;

class Storage {
public:
    friend StorageAccessor;
    friend NodeAccessor;

    Storage();
    ~Storage();

    Storage(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage& operator=(Storage&&) = delete;

    StorageAccessor access();

    StorageAccessor uniqueAccess();

private:
    mutable std::shared_mutex _mainLock;

    std::atomic<uint64_t> _nextFreeNodeID {0};
    std::atomic<uint64_t> _nextFreeEdgeID {0};

    mutable RWSpinLock _nodeMapLock;
    mutable RWSpinLock _edgeMapLock;
    std::map<EntityID, Node*> _nodes;
    std::map<EntityID, Edge*> _edges;

    LabelIndex _labelIndex;
};

}