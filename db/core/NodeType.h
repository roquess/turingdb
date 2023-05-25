// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <set>

#include "DBEntityType.h"

namespace db {

class EdgeType;
class Writeback;

class NodeType : public DBEntityType {
public:
    friend Writeback;

private:
    std::set<EdgeType*, DBObject::Comparator> _inEdgeTypes;
    std::set<EdgeType*, DBObject::Comparator> _outEdgeTypes;

    NodeType(DBIndex index, StringRef name);
    ~NodeType();
};

}
