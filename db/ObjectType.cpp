// Copyright 2023 Turing Biosystems Ltd.

#include "ObjectType.h"

#include "DB.h"
#include "LinkType.h"

using namespace db;

ObjectType::ObjectType(StringRef name)
    : _name(name)
{
}

ObjectType::~ObjectType() {
}

ObjectType* ObjectType::create(DB* db, StringRef name) {
    ObjectType* objType = new ObjectType(name);
    db->addObjectType(objType);
    return objType;
}

void ObjectType::addInLinkType(LinkType* linkType) {
    if (linkType->getTargetType() != this) {
        return;
    }
    _inLinkTypes.push_back(linkType);
}

void ObjectType::addOutLinkType(LinkType* linkType) {
    if (linkType->getSourceType() != this) {
        return;
    }
    _outLinkTypes.push_back(linkType);
}

void ObjectType::addProperty(PropertyType* propType) {
    _propTypes.push_back(propType);
}
