#pragma once

#include <array>
#include <limits.h>

#include "PipelineOpcode.h"

namespace db {

class ExecutionContext;
class Pipeline;

class Executor {
public:
    using PipelineJumpTable = std::array<void*, (uint64_t)PipelineOpcode::MAX>;

    Executor();
    ~Executor();
    bool run(ExecutionContext* ctxt, Pipeline* pipeline);

private:
    PipelineJumpTable _activateTbl;
    PipelineJumpTable _returnTbl;

    void init();
    void runImpl(ExecutionContext* ctxt, Pipeline* pipeline, bool init = false);
    void checkJumpTables();
};

}
