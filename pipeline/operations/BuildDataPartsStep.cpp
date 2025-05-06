#include "BuildDataPartsStep.h"
#include "ChangeManager.h"
#include "JobSystem.h"
#include "Profiler.h"
#include "SystemManager.h"
#include "PipelineException.h"
#include "versioning/Transaction.h"
#include "versioning/CommitBuilder.h"

using namespace db;

BuildDataPartsStep::BuildDataPartsStep()
{
}

BuildDataPartsStep::~BuildDataPartsStep() {
}

void BuildDataPartsStep::prepare(ExecutionContext* ctxt) {
    auto changeRes = ctxt->getSystemManager()->getChangeManager().getChange(ctxt->getCommitHash());
    if (!changeRes) {
        throw PipelineException("No active change matches the requested hash");
    }

    auto& change = *changeRes.value();
    if (change.pendingCount() == 0) {
        throw PipelineException("Build datapart failed: no pending changes");
    }

    _jobSystem = ctxt->getJobSystem();
    _change = &change;
}

void BuildDataPartsStep::execute() {
    Profile profile {"BuildDataPartsStep::execute"};

    _change->buildAllPending(*_jobSystem);
}

void BuildDataPartsStep::describe(std::string& descr) const {
    descr.clear();
    descr += "BuildDataPartsStep";
}
