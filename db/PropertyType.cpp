// Copyright 2023 Turing Biosystems Ltd.

#include "PropertyType.h"

#include "ObjectType.h"
#include "LinkType.h"

using namespace db;

PropertyType::PropertyType(StringRef name, ValueType* valueType)
    : _name(name),
    _valueType(valueType)
{
    _entity = (ObjectType*)nullptr;
}

PropertyType::~PropertyType() {
}

ObjectType* PropertyType::getObjectType() const {
    if (!std::holds_alternative<ObjectType*>(_entity)) {
        return nullptr;
    }
    return std::get<ObjectType*>(_entity);
}

LinkType* PropertyType::getLinkType() const {
    if (!std::holds_alternative<LinkType*>(_entity)) {
        return nullptr;
    }
    return std::get<LinkType*>(_entity);
}

void PropertyType::setObjectType(ObjectType* objType) {
    _entity = objType;
}

void PropertyType::setLinkType(LinkType* linkType) {
    _entity = linkType;
}

bool PropertyType::isAttachedToObject() const {
    return std::holds_alternative<ObjectType*>(_entity);
}

bool PropertyType::isAttachedToLink() const {
    return std::holds_alternative<LinkType*>(_entity);
}

PropertyType* PropertyType::create(ObjectType* objType, StringRef name, ValueType* valueType) {
    PropertyType* propType = new PropertyType(name, valueType);
    propType->setObjectType(objType);
    objType->addProperty(propType);
    return propType;
}

PropertyType* PropertyType::create(LinkType* linkType, StringRef name, ValueType* valueType) {
    PropertyType* propType = new PropertyType(name, valueType);
    propType->setLinkType(linkType);
    linkType->addProperty(propType);
    return propType;
}
