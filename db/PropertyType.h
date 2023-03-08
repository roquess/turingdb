// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_PROPERTY_TYPE_
#define _DB_PROPERTY_TYPE_

#include <variant>

#include "StringRef.h"

namespace db {

class ValueType;
class ObjectType;
class LinkType;

class PropertyType {
public:
    StringRef getName() const { return _name; }

    ValueType* getValueType() const { return _valueType; }

    bool isAttachedToObject() const;
    bool isAttachedToLink() const;
    ObjectType* getObjectType() const;
    LinkType* getLinkType() const;

    std::size_t getIndex() const { return _index; }

    static PropertyType* create(ObjectType* objType, StringRef name, ValueType* type);
    static PropertyType* create(LinkType* linkType, StringRef name, ValueType* type);

private:
    StringRef _name;
    ValueType* _valueType {nullptr};
    std::variant<ObjectType*,LinkType*> _entity;
    std::size_t _index {0};

    PropertyType(StringRef name, ValueType* valueType);
    ~PropertyType();
    void setObjectType(ObjectType* objType);
    void setLinkType(LinkType* linkType);
};

}

#endif
