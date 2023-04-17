#include "ComponentType.h"

#include "Property.h"

using namespace db;

ComponentType::ComponentType(StringRef name)
    : _name(name)
{
}

ComponentType::~ComponentType() {
}

Property* ComponentType::getProperty(StringRef name) const {
    const auto foundIt = _propMap.find(name);
    if (foundIt == _propMap.end()) {
        return nullptr;
    }

    return foundIt->second;
}

void ComponentType::addProperty(Property* prop) {
    _properties.push_back(prop);
    _propMap[prop->getName()] = prop;
}
