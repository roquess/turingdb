#pragma once

#include <vector>
#include <functional>

#include "Symbol.h"
#include "Value.h"

namespace db::query {

class Frame;
class ExecutionContext;

class Cursor {
public:
    virtual bool pull(Frame& frame, ExecutionContext* ctxt) = 0;
};

class LogicalOperator {
public:
    virtual ~LogicalOperator();

    virtual Cursor* makeCursor() = 0;
};

class OutputTableOperator : public LogicalOperator {
public:
    using Callback = std::function<void(Frame&,
                                        ExecutionContext*,
                                        std::vector<std::vector<db::Value>>&)>;

    OutputTableOperator(const std::vector<Symbol>& outputSymbols,
                        Callback callback);
    ~OutputTableOperator();

    Cursor* makeCursor() override;

private:
    std::vector<Symbol> _outputSymbols;
    Callback _callback;

    class OutputTableCursor : public Cursor {
    public:
        OutputTableCursor(OutputTableOperator* self);
        ~OutputTableCursor();

        bool pull(Frame& frame, ExecutionContext* ctxt) override;

    private:
        OutputTableOperator* _self {nullptr};
        std::vector<std::vector<db::Value>> _rows;
        bool _pulled {false};
        size_t _currentRow {0};
    };
};

}
