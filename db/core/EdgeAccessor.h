#pragma once

#include "Result.h"

namespace db {

class StorageAccessor;
class Edge;
class Node;

class EdgeAccessor {
public:
    friend StorageAccessor;

    Result<Node*> getSource() const;

    Result<Node*> getTarget() const;

private:
    Edge* _edge {nullptr};

    EdgeAccessor(Edge* edge);
};

}