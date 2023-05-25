// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <set>
#include <vector>

#include "DBEntityType.h"

namespace db {

class NodeType;
class Writeback;

class EdgeType : public DBEntityType {
public:
    friend Writeback;
    using NodeTypes = std::set<NodeType*, DBObject::Comparator>;

    const NodeTypes& getSourceTypes() const { return _sourceTypes; }
    const NodeTypes& getTargetTypes() const { return _targetTypes; }

private:
    NodeTypes _sourceTypes;
    NodeTypes _targetTypes;

    EdgeType(DBIndex index,
             StringRef name,
             const std::vector<NodeType*>& sources,
             const std::vector<NodeType*>& targets);
    ~EdgeType();
};

}
