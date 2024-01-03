#pragma once

#include <vector>

#include "LabelID.h"
#include "Result.h"

namespace db {

class StorageAccessor;
class Storage;
class Node;
class Edge;
class EdgeMap;

class NodeAccessor {
public:
    friend StorageAccessor;

    NodeAccessor(Node* node, Storage* storage);

    Node* getNode() const { return _node; }

    Result<bool> hasLabel(LabelID label) const;

    // Add a label to the node
    // returns false if the label is already present
    Result<bool> addLabel(LabelID label);

    Result<bool> inEdges(std::vector<Edge*>& edges,
                         const std::vector<LabelID>& edgeTypes) const;

    Result<bool> outEdges(std::vector<Edge*>& edges,
                          const std::vector<LabelID>& edgeTypes) const;

private:
    Node* _node {nullptr};
    Storage* _storage {nullptr};

    Result<bool> getEdges(std::vector<Edge*>& edges,
                          const std::vector<LabelID>& edgeTypes,
                          const EdgeMap& edgeMap) const;
};

}