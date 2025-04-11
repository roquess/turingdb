#include "CallPropertyStep.h"

#include "GraphMetadata.h"
#include "types/PropertyTypeMap.h"

using namespace db;

CallPropertyStep::CallPropertyStep(ColumnVector<PropertyTypeID>* id,
                                   ColumnVector<std::string_view>* propName,
                                   ColumnVector<std::string_view>* propType)
    : _id(id),
    _propName(propName),
    _propType(propType)
{
}

CallPropertyStep::~CallPropertyStep() {
}

void CallPropertyStep::execute() {
    _id->clear();
    _propName->clear();
    _propType->clear();

    const PropertyTypeMap& propTypeMap = _view->metadata().propTypes();
    const std::unordered_map<std::string, size_t>& offsetMap = propTypeMap._offsetMap;

    for (const auto& entry : offsetMap) {
        _propName->emplace_back(entry.first);
        const auto propType = propTypeMap.get(entry.first);
        _id->emplace_back(propType._id);
        _propType->emplace_back(PropertyValueTypeDescription::value(propType._valueType));
    };
}

void CallPropertyStep::describe(std::string& descr) const {
    descr.assign("CallPropertyStep");
}
