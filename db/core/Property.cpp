#include "Property.h"

using namespace db;

Property::Property(const PropertyType* type, Value value)
    : _type(type),
    _value(value)
{
}

Property::Property()
{
}

Property::~Property() {
}
