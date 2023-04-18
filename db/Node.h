// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <unordered_map>

#include "StringRef.h"
#include "Value.h"

namespace db {

class NodeType;
class NodeDescriptor;
class Property;
class Writeback;

class Node {
public:
    friend Writeback;

    Value getProperty(const Property* prop) const;

private:
    NodeDescriptor* _desc {nullptr};
    std::unordered_map<StringRef, Value> _properties;

    Node(NodeDescriptor* desc);
    ~Node();
    void setDescriptor(NodeDescriptor* desc);
};

}
