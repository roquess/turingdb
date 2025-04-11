#pragma once

#include <string>

#include "ExecutionContext.h"
#include "columns/ColumnVector.h"
#include "views/GraphView.h"

namespace db {

class ExecutionContext;

class CallPropertyStep {
public:
    struct Tag {};

    explicit CallPropertyStep(ColumnVector<PropertyTypeID>* id,
                              ColumnVector<std::string_view>* propName,
                              ColumnVector<std::string_view>* propType);
    ~CallPropertyStep();

    void prepare(ExecutionContext* ctxt) {
        _view = &ctxt->getGraphView();
    }

    bool isFinished() const { return true; }

    void reset() {}

    void execute();

    void describe(std::string& descr) const;

private:
    const GraphView* _view {nullptr};
    ColumnVector<PropertyTypeID>* _id {nullptr};
    ColumnVector<std::string_view>* _propName {nullptr};
    ColumnVector<std::string_view>* _propType {nullptr};
};

}
