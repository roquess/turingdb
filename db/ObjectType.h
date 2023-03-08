// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_OBJECT_TYPE_
#define _DB_OBJECT_TYPE_

#include <vector>

#include "StringRef.h"

namespace db {

class DB;
class LinkType;
class PropertyType;

class ObjectType {
public:
    friend DB;
    friend LinkType;
    friend PropertyType;
    using LinkTypes = std::vector<LinkType*>;
    using PropertyTypes = std::vector<PropertyType*>;

    StringRef getName() const { return _name; }

    const PropertyTypes& getPropertyTypes() const { return _propTypes; }
    const LinkTypes& getInLinkTypes() const { return _inLinkTypes; }
    const LinkTypes& getOutLinkTypes() const { return _outLinkTypes; }

    static ObjectType* create(DB* db, StringRef name);

private:
    StringRef _name;
    LinkTypes _inLinkTypes;
    LinkTypes _outLinkTypes;
    PropertyTypes _propTypes;

    ObjectType(StringRef name);
    ~ObjectType();
    void addInLinkType(LinkType* linkType);
    void addOutLinkType(LinkType* linkType);
    void addProperty(PropertyType* propType);
};

}

#endif
