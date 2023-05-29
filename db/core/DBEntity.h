#pragma once

#include <map>

#include "DBObject.h"

#include "Property.h"

namespace db {

class PropertyType;
class DBEntityType;
class Writeback;

class DBEntity : public DBObject {
public:
    friend Writeback;

    Property getProperty(const PropertyType* propType) const;

    DBEntityType* getType() const { return _type; }

protected:
    DBEntity(DBIndex index, DBEntityType* type);
    virtual ~DBEntity();

private:
    using Properties = std::map<const PropertyType*, Value, DBObject::Comparator>;
    DBEntityType* _type {nullptr};
    Properties _properties;

    void addProperty(const Property& prop);
};

}
