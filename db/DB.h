// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <map>

#include "StringIndex.h"
#include "StringRef.h"

namespace db {

class ValueType;
class Network;
class Writeback;

class DB {
public:
    friend Writeback;

    ~DB();

    static DB* create();

    StringRef getString(const std::string& str);

    // Value types
    ValueType* getInt() const { return _int; }
    ValueType* getUnsigned() const { return _unsigned; }
    ValueType* getBool() const { return _bool; }
    ValueType* getDecimal() const { return _decimal; }
    ValueType* getString() const { return _string; }

    Network* getNetwork(StringRef name) const;

private:
    StringIndex _strIndex;
    std::map<StringRef, Network*, StringRef::Comparator> _networks;

    ValueType* _int {nullptr};
    ValueType* _unsigned {nullptr};
    ValueType* _bool {nullptr};
    ValueType* _decimal {nullptr};
    ValueType* _string {nullptr};

    DB();
    void addNetwork(Network* net);
};

}
