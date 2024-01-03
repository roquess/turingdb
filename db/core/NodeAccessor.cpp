#include "NodeAccessor.h"

#include <shared_mutex>
#include <mutex>
#include <algorithm>

#include "Node.h"
#include "Storage.h"

using namespace db;

NodeAccessor::NodeAccessor(Node* node, Storage* storage)
    : _node(node),
    _storage(storage)
{
}

Result<bool> NodeAccessor::hasLabel(LabelID label) const {
    bool deleted = false;
    bool hasLabel = false;

    // Get node data
    {
        std::shared_lock guard(_node->_lock);
        deleted = _node->_deleted;
        const auto& labels = _node->_labels;
        hasLabel = std::find(labels.begin(), labels.end(), label) != labels.end();
    }

    if (deleted) {
        return Error::DELETED_OBJECT;
    }

    return hasLabel;
}

Result<bool> NodeAccessor::addLabel(LabelID label) {
    std::unique_lock guard(_node->_lock);

    if (_node->_deleted) {
        return Error::DELETED_OBJECT;
    }

    const auto& labels = _node->_labels;
    if (std::find(labels.begin(), labels.end(), label) != labels.end()) {
        return false;
    }

    _node->_labels.emplace_back(label);
    _storage->_labelIndex.updateOnAddLabel(label, _node);

    return true;
}

Result<bool> NodeAccessor::getEdges(std::vector<Edge*>& edges,
                                    const std::vector<LabelID>& edgeTypes,
                                    const EdgeMap& edgeMap) const {
    std::shared_lock guard(_node->_lock);

    if (_node->_deleted) {
        return Error::DELETED_OBJECT;
    }

    if (edgeMap.empty()) {
        for (const auto& entry : edgeMap) {
            const auto& toInsert = entry.second;
            edges.reserve(edges.size()+toInsert.size());
            edges.insert(edges.end(), toInsert.begin(), toInsert.end());
        }
    } else {
        for (LabelID labelID : edgeTypes) {
            const auto it = edgeMap.find(labelID);
            if (it == edgeMap.end()) {
                continue;
            }

            const auto& toInsert = it->second;
            edges.reserve(edges.size()+toInsert.size());
            edges.insert(edges.end(), toInsert.begin(), toInsert.end());
        }
    }

    return true;
}

Result<bool> NodeAccessor::inEdges(std::vector<Edge*>& edges,
                                   const std::vector<LabelID>& edgeTypes) const {
    return getEdges(edges, edgeTypes, _node->_inEdges);
}

Result<bool> NodeAccessor::outEdges(std::vector<Edge*>& edges,
                                    const std::vector<LabelID>& edgeTypes) const {
    return getEdges(edges, edgeTypes, _node->_outEdges);
}