// Copyright 2023 Turing Biosystems Ltd.

#include "Object.h"

#include "Network.h"
#include "LinkStore.h"
#include "ObjectType.h"
#include "Link.h"
#include "LinkType.h"
#include "PropertyType.h"
#include "PropertyStore.h"

using namespace db;

Object::Object(Network* net, ObjectType* type, StringRef name)
    : _net(net),
    _type(type),
    _displayName(name)
{
    _properties = new PropertyStore(type->getPropertyTypes().size());
    _inLinks = new LinkStore(type->getInLinkTypes().size());
    _outLinks = new LinkStore(type->getOutLinkTypes().size());
}

Object::~Object() {
    if (_properties) {
        delete _properties;
    }

    if (_inLinks) {
        delete _inLinks;
    }
    if (_outLinks) {
        delete _outLinks;
    }
}

Object* Object::create(Network* net, ObjectType* type, StringRef displayName) {
    Object* obj = new Object(net, type, displayName);
    net->addObject(obj);

    return obj;
}

void Object::addInLink(Link* link) {
    if (link->getTarget() != this) {
        return;
    }
    const std::size_t linkIndex = link->getType()->getTargetIndex();
    _inLinks->addLink(link, linkIndex);
}

void Object::addOutLink(Link* link) {
    if (link->getSource() != this) {
        return;
    }
    const std::size_t linkIndex = link->getType()->getSourceIndex();
    _outLinks->addLink(link, linkIndex);
}

Value Object::getProperty(const PropertyType* prop) const {
    if (prop->getObjectType() != _type) {
        return Value();
    }
    return _properties->getProperty(prop->getIndex());
}

void Object::setProperty(const PropertyType* prop, Value val) {
    if (prop->getObjectType() != _type) {
        return;
    }
    _properties->setProperty(prop->getIndex(), val);
}
