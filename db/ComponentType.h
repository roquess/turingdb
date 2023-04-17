#pragma once

#include <vector>
#include <unordered_map>

#include "StringRef.h"

namespace db {

class Property;
class Writeback;

class ComponentType {
public:
    friend Writeback;

    StringRef getName() const { return _name; }

    Property* getProperty(StringRef name) const;

private:
    StringRef _name;
    std::vector<Property*> _properties;
    std::unordered_map<StringRef, Property*> _propMap;

    ComponentType(StringRef name);
    ~ComponentType();
    void addProperty(Property* prop);
};

}
