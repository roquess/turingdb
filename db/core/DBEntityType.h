#pragma once

#include <map>

#include "DBType.h"

#include "StringRef.h"
#include "Range.h"

namespace db {

class PropertyType;
class Writeback;

class DBEntityType : public DBType {
public:
    friend Writeback;
    using PropertyTypes = std::map<StringRef, PropertyType*>;
    using PropertyTypeRange = STLIndexRange<PropertyTypes>;

    PropertyType* getPropertyType(StringRef name) const;

    PropertyTypeRange propertyTypes() const;

protected:
    DBEntityType(DBIndex index, StringRef name);
    virtual ~DBEntityType();
    void addPropertyType(PropertyType* propType);

private:
    PropertyTypes _propTypes;
};

}
