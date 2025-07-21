#include "CallLabelStep.h"

#include "metadata/GraphMetadata.h"
#include "metadata/LabelMap.h"

using namespace db;

CallLabelStep::CallLabelStep(ColumnVector<LabelID>* id,
                                   ColumnVector<std::string_view>* labelName)
    : _id(id),
    _labelName(labelName)
{
}

CallLabelStep::~CallLabelStep() {
}

void CallLabelStep::execute() {
    _id->clear();
    _labelName->clear();

    const LabelMap& labelMap = _view->metadata().labels();

    for (const auto& entry : labelMap) {
        _id->emplace_back(entry._id);
        _labelName->emplace_back(*entry._name);
    };
}

void CallLabelStep::describe(std::string& descr) const {
    descr.assign("CallLabelStep");
}
