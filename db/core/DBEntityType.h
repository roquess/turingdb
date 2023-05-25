#pragma once

#include <map>

#include "DBType.h"

#include "StringRef.h"

namespace db {

class PropertyType;
class Writeback;

class DBEntityType : public DBType {
public:
    friend Writeback;

    PropertyType* getPropertyType(StringRef name) const;

protected:
    DBEntityType(DBIndex index, StringRef name);
    virtual ~DBEntityType();
    void addPropertyType(PropertyType* propType);

private:
    std::map<StringRef, PropertyType*> _propTypes;
};

}
