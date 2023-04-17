#include "Property.h"

using namespace db;

Property::Property(StringRef name,
                   ValueType* valType,
                   ComponentType* compType)
    : _name(name),
    _valType(valType),
    _compType(compType)
{
}

Property::~Property() {
}
