#include "LoadGraphStep.h"

#include <sstream>

#include "ExecutionContext.h"
#include "Profiler.h"
#include "SystemManager.h"
#include "PipelineException.h"

using namespace db;

LoadGraphStep::LoadGraphStep(const std::string& graphName)
    : _graphName(graphName)
{
}

LoadGraphStep::~LoadGraphStep() {
}

void LoadGraphStep::prepare(ExecutionContext* ctxt) {
    _sysMan = ctxt->getSystemManager();
    _jobSystem = ctxt->getJobSystem();
}

void LoadGraphStep::execute() {
    Profile profile {"LoadGraphStep::execute"};

    if (!_sysMan->loadGraph(_graphName, *_jobSystem)) {
        throw PipelineException("Failed to load graph " + _graphName);
    }
} 

void LoadGraphStep::describe(std::string& descr) const {
    std::stringstream ss;
    ss << "LoadGraphStep";
    ss << " graphName=" << _graphName;
    descr.assign(ss.str());
}
