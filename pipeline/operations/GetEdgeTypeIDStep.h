#pragma once

#include <string>

#include "ExecutionContext.h"
#include "columns/ColumnIDs.h"

namespace db {

class ExecutionContext;

class GetEdgeTypeIDStep {

public:
    struct Tag {};

    GetEdgeTypeIDStep(const ColumnIDs* edgeIDs, ColumnVector<EdgeTypeID>* edgeTypeIDs);
    ~GetEdgeTypeIDStep();

    void prepare(ExecutionContext* ctxt){
        _view = &ctxt->getGraphView();
    }

    void reset() {}

    inline bool isFinished() const { return true; }

    void execute();

    void describe(std::string& descr) const;

private:
    const ColumnIDs* _edgeIDs {nullptr};
    ColumnVector<EdgeTypeID>* _edgeTypeIDs {nullptr};
    const GraphView* _view {nullptr};

};

}
