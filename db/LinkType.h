// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_LINK_TYPE_
#define _DB_LINK_TYPE_

#include <vector>

#include "StringRef.h"

namespace db {

class DB;
class ObjectType;
class PropertyType;

class LinkType {
public:
    friend DB;
    friend PropertyType;
    using PropertyTypes = std::vector<PropertyType*>;

    StringRef getName() const { return _name; }

    ObjectType* getSourceType() const { return _sourceType; }
    ObjectType* getTargetType() const { return _targetType; }

    std::size_t getSourceIndex() const { return _sourceIndex; }
    std::size_t getTargetIndex() const { return _targetIndex; }

    static LinkType* create(DB* db,
                            StringRef name,
                            ObjectType* source,
                            ObjectType* target);

private:
    StringRef _name;
    ObjectType* _sourceType {nullptr};
    ObjectType* _targetType {nullptr};
    std::size_t _sourceIndex {0};
    std::size_t _targetIndex {0};
    PropertyTypes _propTypes;

    LinkType(StringRef name,
             ObjectType* sourceType,
             ObjectType* targetType,
             std::size_t sourceIndex,
             std::size_t targetIndex);
    ~LinkType();
    void addProperty(PropertyType* prop);
};

}

#endif
