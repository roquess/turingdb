#pragma once

#include <string>

#include "ExecutionContext.h"
#include "columns/ColumnVector.h"
#include "views/GraphView.h"

namespace db {

class ExecutionContext;

class CallLabelStep {
public:
    struct Tag {};

    explicit CallLabelStep(ColumnVector<LabelID>* id,
                           ColumnVector<std::string_view>* labelName);
    ~CallLabelStep();

    void prepare(ExecutionContext* ctxt) {
        _view = &ctxt->getGraphView();
    }

    bool isFinished() const { return true; }

    void reset() {}

    void execute();

    void describe(std::string& descr) const;

private:
    const GraphView* _view {nullptr};
    ColumnVector<LabelID>* _id {nullptr};
    ColumnVector<std::string_view>* _labelName {nullptr};
};

}
