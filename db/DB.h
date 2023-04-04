// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_DB_
#define _DB_DB_

#include <vector>

#include "StringIndex.h"

namespace db {

class ValueType;
class Network;

class DB {
public:
    friend Network;
    using Networks = std::vector<Network*>;

    ~DB();

    static DB* create();

    StringRef getString(const std::string& str);

    // Value types
    ValueType* getInt() const { return _int; }
    ValueType* getUnsigned() const { return _unsigned; }
    ValueType* getBool() const { return _bool; }
    ValueType* getDecimal() const { return _decimal; }
    ValueType* getString() const { return _string; }

private:
    StringIndex _strIndex;
    Networks _networks;

    ValueType* _int {nullptr};
    ValueType* _unsigned {nullptr};
    ValueType* _bool {nullptr};
    ValueType* _decimal {nullptr};
    ValueType* _string {nullptr};

    DB();
    void addNetwork(Network* net);
};

}

#endif
