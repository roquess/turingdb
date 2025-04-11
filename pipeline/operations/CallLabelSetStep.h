#pragma once

#include <string>

#include "ExecutionContext.h"
#include "columns/ColumnVector.h"
#include "views/GraphView.h"

namespace db {

class ExecutionContext;

class CallLabelSetStep {
public:
    struct Tag {};

    explicit CallLabelSetStep(ColumnVector<LabelSetID>* id,
                              ColumnVector<std::string>* labelNames);
    ~CallLabelSetStep();

    void prepare(ExecutionContext* ctxt) {
        _view = &ctxt->getGraphView();
    }

    bool isFinished() const { return true; }

    void reset() {}

    void execute();

    void describe(std::string& descr) const;

private:
    const GraphView* _view {nullptr};
    ColumnVector<LabelSetID>* _id {nullptr};
    ColumnVector<std::string>* _labelNames {nullptr};
};

}
