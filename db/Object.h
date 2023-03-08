// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_OBJECT_
#define _DB_OBJECT_

#include "StringRef.h"
#include "Value.h"

namespace db {

class Network;
class ObjectType;
class PropertyStore;
class LinkStore;
class Link;
class PropertyType;

class Object {
public:
    friend Link;
    friend Network;

    StringRef getDisplayName() const { return _displayName; }
    
    ObjectType* getType() const { return _type; }

    Network* getNetwork() { return _net; }

    Value getProperty(const PropertyType* prop) const;
    void setProperty(const PropertyType* prop, Value val);

    static Object* create(Network* net, ObjectType* type, StringRef name);

private:
    Network* _net {nullptr};
    ObjectType* _type {nullptr};
    StringRef _displayName;
    PropertyStore* _properties {nullptr};
    LinkStore* _inLinks {nullptr};
    LinkStore* _outLinks {nullptr};

    Object(Network* net, ObjectType* type, StringRef displayName);
    ~Object();
    void addInLink(Link* link);
    void addOutLink(Link* link);
};

}

#endif
