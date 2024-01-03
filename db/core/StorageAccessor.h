#pragma once

#include <mutex>
#include <shared_mutex>

#include "EntityID.h"
#include "LabelID.h"
#include "Result.h"

namespace db {

class Storage;
class Node;
class Edge;

class StorageAccessor {
public:
    friend Storage;

    struct SharedAccess {};
    struct UniqueAccess {};

    StorageAccessor(StorageAccessor&& other);
    ~StorageAccessor();

    StorageAccessor(const StorageAccessor&) = delete;
    StorageAccessor& operator=(const StorageAccessor&) = delete;
    StorageAccessor& operator=(StorageAccessor &&) = delete;

    Node* getNode(EntityID id) const;
    Edge* getEdge(EntityID id) const;

    Node* createNode();

    Result<Edge*> createEdge(Node* source, Node* target, LabelID label);
    
private:
    Storage* _storage {nullptr};
    std::unique_lock<std::shared_mutex> _uniqueLock;
    std::shared_lock<std::shared_mutex> _sharedLock;

    StorageAccessor(SharedAccess, Storage* storage);
    StorageAccessor(UniqueAccess, Storage* storage);
};

}