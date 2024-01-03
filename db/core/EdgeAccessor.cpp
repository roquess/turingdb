#include "EdgeAccessor.h"

#include <mutex>

#include "Edge.h"

using namespace db;

EdgeAccessor::EdgeAccessor(Edge* edge)
    : _edge(edge)
{
}

Result<Node*> EdgeAccessor::getSource() const {
    bool deleted = false;
    Node* source = nullptr;

    {
        std::unique_lock guard(_edge->_lock);
        deleted = _edge->_deleted;
        source = _edge->_source;
    }

    if (deleted) {
        return Error::DELETED_OBJECT;
    }

    return source;
}

Result<Node*> EdgeAccessor::getTarget() const {
    bool deleted = false;
    Node* target = nullptr;

    {
        std::unique_lock guard(_edge->_lock);
        deleted = _edge->_deleted;
        target = _edge->_target;
    }

    if (deleted) {
        return Error::DELETED_OBJECT;
    }

    return target;
}