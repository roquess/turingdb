#include "CommitStep.h"

#include <sstream>

#include "ExecutionContext.h"
#include "Profiler.h"
#include "versioning/CommitBuilder.h"
#include "SystemManager.h"
#include "PipelineException.h"

using namespace db;

CommitStep::CommitStep() = default;

CommitStep::~CommitStep() = default;

void CommitStep::prepare(ExecutionContext* ctxt) {
    //auto* sysMan = ctxt->getSystemManager();
}

void CommitStep::execute() {
    //auto res = _commitBuilder->submit(*_jobSystem);
    //if (!res) {
    //    throw PipelineException("CommitStep: Failed to commit: {}", res.error().fmtMessage());
    //}
}

void CommitStep::describe(std::string& descr) const {
    std::stringstream ss;
    ss << "CommitStep";
    descr.assign(ss.str());
}
