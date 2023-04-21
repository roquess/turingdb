#pragma once

#include "StringRef.h"

namespace db {

class ComponentType;
class ValueType;
class Writeback;

class Property {
public:
    friend Writeback;

    StringRef getName() const { return _name; }

    ValueType* getValueType() const { return _valType; }

    ComponentType* getComponentType() const { return _compType; }

private:
    StringRef _name;
    ValueType* _valType {nullptr};
    ComponentType* _compType {nullptr};

    Property(StringRef name,
             ValueType* valType,
             ComponentType* compType);
    ~Property();
};

}
