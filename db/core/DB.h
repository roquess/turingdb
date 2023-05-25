// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <map>

#include "StringIndex.h"
#include "StringRef.h"
#include "DBIndex.h"

namespace db {

class ValueType;
class Network;
class NodeType;
class EdgeType;
class DBNetworkRange;
class DBNodeTypeRange;
class DBEdgeTypeRange;
class Writeback;
class DBLoader;
class DBDumper;

class DB {
public:
    friend DBNetworkRange;
    friend DBNodeTypeRange;
    friend DBEdgeTypeRange;
    friend Writeback;
    friend DBLoader;
    friend DBDumper;

    ~DB();

    static DB* create();

    StringRef getString(const std::string& str);

    // Value types
    ValueType* getIntType() const { return _int; }
    ValueType* getUnsignedType() const { return _unsigned; }
    ValueType* getBoolType() const { return _bool; }
    ValueType* getDecimalType() const { return _decimal; }
    ValueType* getStringType() const { return _string; }

    NodeType* getNodeType(StringRef name) const;
    EdgeType* getEdgeType(StringRef name) const;

    Network* getNetwork(StringRef name) const;

private:
    using Networks = std::map<StringRef, Network*>;
    using NodeTypes = std::map<StringRef, NodeType*>;
    using EdgeTypes = std::map<StringRef, EdgeType*>;

    StringIndex _strIndex;
    Networks _networks;
    NodeTypes _nodeTypes;
    EdgeTypes _edgeTypes;

    ValueType* _int {nullptr};
    ValueType* _unsigned {nullptr};
    ValueType* _bool {nullptr};
    ValueType* _decimal {nullptr};
    ValueType* _string {nullptr};

    DBIndex::ID _nextFreeNetID {0};
    DBIndex::ID _nextFreeNodeID {0};
    DBIndex::ID _nextFreeEdgeID {0};
    DBIndex::ID _nextFreeNodeTypeID {0};
    DBIndex::ID _nextFreeEdgeTypeID {0};
    DBIndex::ID _nextFreePropertyTypeID {0};

    DB();
    void addNetwork(Network* net);
    void addNodeType(NodeType* nodeType);
    void addEdgeType(EdgeType* edgeType);
    DBIndex allocNetworkIndex();
    DBIndex allocNodeIndex();
    DBIndex allocEdgeIndex();
    DBIndex allocNodeTypeIndex();
    DBIndex allocEdgeTypeIndex();
    DBIndex allocPropertyTypeIndex();
};

}
