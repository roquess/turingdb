#pragma once

#include <map>

#include "DBObject.h"

#include "Property.h"

namespace db {

class PropertyType;
class Writeback;

class DBEntity : public DBObject {
public:
    friend Writeback;

    Property getProperty(const PropertyType* propType) const;

protected:
    DBEntity(DBIndex index);
    virtual ~DBEntity();

private:
    std::map<const PropertyType*, Value, DBObject::Comparator> _properties;

    void addProperty(const Property& prop);
};

}
