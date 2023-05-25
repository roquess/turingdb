// Copyright 2023 Turing Biosystems Ltd.

#include "EdgeType.h"

#include "NodeType.h"

using namespace db;

EdgeType::EdgeType(DBIndex index,
                   StringRef name,
                   const std::vector<NodeType*>& sources,
                   const std::vector<NodeType*>& targets)
    : DBEntityType(index, name),
    _sourceTypes(sources.begin(), sources.end()),
    _targetTypes(targets.begin(), targets.end())
{
}

EdgeType::~EdgeType() {
}
