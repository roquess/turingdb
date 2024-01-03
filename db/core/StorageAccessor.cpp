#include "StorageAccessor.h"

#include <mutex>

#include "Storage.h"
#include "Node.h"
#include "Edge.h"

using namespace db;

StorageAccessor::StorageAccessor(SharedAccess, Storage* storage)
    : _storage(storage),
    // We only hold a shared lock on the main lock
    _uniqueLock(storage->_mainLock, std::defer_lock),
    _sharedLock(storage->_mainLock)
{
}

StorageAccessor::StorageAccessor(UniqueAccess, Storage* storage)
    : _storage(storage),
    // We hold an exclusive lock on the main lock
    _uniqueLock(storage->_mainLock),
    _sharedLock(storage->_mainLock, std::defer_lock)
{
}

StorageAccessor::StorageAccessor(StorageAccessor&& other)
    : _storage(other._storage),
    _uniqueLock(std::move(other._uniqueLock)),
    _sharedLock(std::move(other._sharedLock))
{
}

StorageAccessor::~StorageAccessor() {
}

Node* StorageAccessor::getNode(EntityID id) const {
    const auto& nodes = _storage->_nodes;
    const auto it = nodes.find(id);
    if (it == nodes.end()) {
        return nullptr;
    }

    return it->second;
}

Edge* StorageAccessor::getEdge(EntityID id) const {
    const auto& edges = _storage->_edges;
    const auto it = edges.find(id);
    if (it == edges.end()) {
        return nullptr;
    }

    return it->second;
}

Node* StorageAccessor::createNode() {
    const auto id = _storage->_nextFreeNodeID.fetch_add(1, std::memory_order_acq_rel);
    Node* node = new Node(id);

    {
        std::unique_lock guard(_storage->_nodeMapLock);
        _storage->_nodes[id] = node;
    }

    return node;
}

Result<Edge*> StorageAccessor::createEdge(Node* source, Node* target, LabelID label) {
    std::unique_lock guardSource(source->_lock);
    std::unique_lock guardTarget(target->_lock);

    if (source->_deleted || target->_deleted) {
        return Error::DELETED_OBJECT;
    }

    const auto id = _storage->_nextFreeEdgeID.fetch_add(1, std::memory_order_acq_rel);
    Edge* edge = new Edge(id, source, target, label);

    {
        std::unique_lock guard(_storage->_edgeMapLock);
        _storage->_edges[id] = edge;
    }

    source->_outEdges.insert(label, edge);
    target->_inEdges.insert(label, edge);
    _storage->_labelIndex.addEdge(label, edge);

    return edge;
}