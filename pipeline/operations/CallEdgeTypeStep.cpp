#include "CallEdgeTypeStep.h"

#include "metadata/GraphMetadata.h"
#include "metadata/EdgeTypeMap.h"

using namespace db;

CallEdgeTypeStep::CallEdgeTypeStep(ColumnVector<EdgeTypeID>* id,
                                   ColumnVector<std::string_view>* edgeTypeName)
    : _id(id),
    _edgeTypeName(edgeTypeName)
{
}

CallEdgeTypeStep::~CallEdgeTypeStep() {
}

void CallEdgeTypeStep::execute() {
    _id->clear();
    _edgeTypeName->clear();

    const EdgeTypeMap& edgeTypeMap = _view->metadata().edgeTypes();

    for (const auto& entry : edgeTypeMap) {
        _id->emplace_back(entry._id);
        _edgeTypeName->emplace_back(*entry._name);
    };
}

void CallEdgeTypeStep::describe(std::string& descr) const {
    descr.assign("CallEdgeTypeStep");
}
