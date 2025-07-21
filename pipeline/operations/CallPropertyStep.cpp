#include "CallPropertyStep.h"

#include "metadata/GraphMetadata.h"
#include "metadata/PropertyTypeMap.h"

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

    for (const auto& entry : propTypeMap) {
        _propName->emplace_back(*entry._name);
        const auto propType = entry._pt;
        _id->emplace_back(propType._id);
        _propType->emplace_back(ValueTypeName::value(propType._valueType));
    };
}

void CallPropertyStep::describe(std::string& descr) const {
    descr.assign("CallPropertyStep");
}
