// Copyright 2023 Turing Biosystems Ltd.

#include "NodeType.h"

using namespace db;

NodeType::NodeType(DBIndex index, StringRef name)
    : DBEntityType(index, name)
{
}

NodeType::~NodeType() {
}
