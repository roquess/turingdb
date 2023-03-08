// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_DB_
#define _DB_DB_

#include <vector>

#include "StringIndex.h"

namespace db {

class ValueType;
class Network;
class Link;
class ObjectType;
class LinkType;

class DB {
public:
    friend Network;
    friend Link;
    friend ObjectType;
    friend LinkType;
    using Networks = std::vector<Network*>;
    using Links = std::vector<Link*>;

    ~DB();

    static DB* create();

    StringRef getString(const std::string& str);

    // Types
    ValueType* getInt() const { return _int; }
    ValueType* getUnsigned() const { return _unsigned; }
    ValueType* getBool() const { return _bool; }
    ValueType* getDecimal() const { return _decimal; }
    ValueType* getString() const { return _string; }

private:
    using ObjectTypes = std::vector<ObjectType*>;
    using LinkTypes = std::vector<LinkType*>;

    StringIndex _strIndex;
    Networks _networks;
    Links _links;
    ObjectTypes _objTypes;
    LinkTypes _linkTypes;

    ValueType* _int {nullptr};
    ValueType* _unsigned {nullptr};
    ValueType* _bool {nullptr};
    ValueType* _decimal {nullptr};
    ValueType* _string {nullptr};

    DB();
    void addNetwork(Network* net);
    void addLink(Link* link);
    void addObjectType(ObjectType* obj);
    void addLinkType(LinkType* link);
};

}

#endif
