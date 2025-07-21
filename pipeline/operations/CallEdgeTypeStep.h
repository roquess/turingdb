#pragma once

#include <string>

#include "ExecutionContext.h"
#include "columns/ColumnVector.h"
#include "views/GraphView.h"

namespace db {

class ExecutionContext;

class CallEdgeTypeStep {
public:
    struct Tag {};

    explicit CallEdgeTypeStep(ColumnVector<EdgeTypeID>* id,
                              ColumnVector<std::string_view>* edgeTypeName);
    ~CallEdgeTypeStep();

    void prepare(ExecutionContext* ctxt) {
        _view = &ctxt->getGraphView();
    }

    bool isFinished() const { return true; }

    void reset() {}

    void execute();

    void describe(std::string& descr) const;

private:
    const GraphView* _view {nullptr};
    ColumnVector<EdgeTypeID>* _id {nullptr};
    ColumnVector<std::string_view>* _edgeTypeName {nullptr};
};

}
