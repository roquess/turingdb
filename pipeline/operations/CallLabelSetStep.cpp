#include "CallLabelSetStep.h"

#include "metadata/GraphMetadata.h"
#include "metadata/LabelSetMap.h"

using namespace db;

CallLabelSetStep::CallLabelSetStep(ColumnVector<LabelSetID>* id,
                                   ColumnVector<std::string_view>* labelNames)
    : _id(id),
    _labelNames(labelNames)
{
}

CallLabelSetStep::~CallLabelSetStep() {
}

void CallLabelSetStep::execute() {
    _id->clear();
    _labelNames->clear();

    const LabelSetMap& labelSetMap = _view->metadata().labelsets();

    const LabelMap& labelMap = _view->metadata().labels();

    for (const auto& entry : labelSetMap) {
        // Loop through all the existing labelIds to 'decompose' our labelset
        for (const auto& labelEntry : labelMap) {
            if (entry._value->hasLabel(labelEntry._id)) {
                _id->emplace_back(entry._id);
                _labelNames->emplace_back(*labelEntry._name);
            }
        }
    }
}

void CallLabelSetStep::describe(std::string& descr) const {
    descr.assign("CallLabelSetStep");
}
