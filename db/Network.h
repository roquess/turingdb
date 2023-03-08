// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_NETWORK_
#define _DB_NETWORK_

#include <vector>

#include "StringRef.h"

namespace db {

class DB;
class Object;
class Link;

class Network {
public:
    friend DB;
    friend Object;
    friend Link;
    using Networks = std::vector<Network*>;
    using Objects = std::vector<Object*>;
    using Links = std::vector<Link*>;

    StringRef getDisplayName() const { return _displayName; }

    Network* getParent() const { return _parent; }

    const Networks& subNetworks() const { return _subNets; }

    const Objects& objects() const { return _objects; }

    const Links& internalLinks() const { return _internalLinks; }
    const Links& inLinks() const { return _inLinks; }
    const Links& outLinks() const { return _outLinks; }

    static Network* create(DB* db, StringRef displayName);
    static Network* create(DB* db, Network* parent, StringRef displayName);

private:
    StringRef _displayName;
    Networks _subNets;
    Objects _objects;
    Links _internalLinks;
    Links _inLinks;
    Links _outLinks;
    Network* _parent {nullptr};

    Network(StringRef displayName);
    ~Network();
    void setParent(Network* parent);
    void addSubNet(Network* subNet);
    void addObject(Object* obj);
    void addInternalLink(Link* link);
    void addInLink(Link* link);
    void addOutLink(Link* link);
};

}

#endif
