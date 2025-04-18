#include "ChangeStep.h"

#include <sstream>

#include "ChangeManager.h"
#include "ExecutionContext.h"
#include "SystemManager.h"
#include "PipelineException.h"
#include "ChangeCommand.h"

using namespace db;

ChangeStep::ChangeStep(ChangeOpType type, ColumnVector<const CommitBuilder*>* list)
    : _type(type),
      _changeList(list)
{
}

ChangeStep::~ChangeStep() {
}

void ChangeStep::prepare(ExecutionContext* ctxt) {
    _sysMan = ctxt->getSystemManager();
    _view = ctxt->getGraphView();

    if (_type == ChangeOpType::NEW) {
        _changeInfo = std::string{ctxt->getGraphName()};
    } else {
        _changeInfo = ctxt->getCommitHash();
    }
}

void ChangeStep::execute() {
    switch (_type) {
        case ChangeOpType::NEW:
            if (auto res = createChange(); !res) {
                throw PipelineException(fmt::format("Failed to create change: {}", res.error().fmtMessage()));
            }
            break;
        case ChangeOpType::SUBMIT:
            if (auto res = acceptChange(); !res) {
                throw PipelineException(fmt::format("Failed to submit change: {}", res.error().fmtMessage()));
            }
            break;
        case ChangeOpType::DELETE:
            if (auto res = deleteChange(); !res) {
                throw PipelineException(fmt::format("Failed to delete change: {}", res.error().fmtMessage()));
            }
            break;
        case ChangeOpType::LIST:
            listChanges();
            break;
        case ChangeOpType::_SIZE:
            throw PipelineException("ChangeStep: Unknown change type");
    }
}

void ChangeStep::describe(std::string& descr) const {
    std::stringstream ss;
    ss << "ChangeStep";
    ss << " changeType=" << ChangeOpTypeName::value(_type);
    descr.assign(ss.str());
}

ChangeResult<CommitHash> ChangeStep::createChange() const {
    if (!std::holds_alternative<std::string>(_changeInfo)) {
        throw PipelineException("ChangeStep: Change info must contain the grpah name");
    }

    const auto& graphName = std::get<std::string>(_changeInfo);
    return _sysMan->newChange(graphName);
}

ChangeResult<void> ChangeStep::acceptChange() const {
    if (!std::holds_alternative<CommitHash>(_changeInfo)) {
        throw PipelineException("ChangeStep: Change info must contain the change hash");
    }

    CommitHash changeHash = std::get<CommitHash>(_changeInfo);
    return _sysMan->getChangeManager().acceptChange(changeHash);
}

ChangeResult<void> ChangeStep::deleteChange() const {
    if (!std::holds_alternative<CommitHash>(_changeInfo)) {
        throw PipelineException("ChangeStep: Change info must contain the change hash");
    }

    CommitHash changeHash = std::get<CommitHash>(_changeInfo);
    return _sysMan->getChangeManager().deleteChange(changeHash);
}

void ChangeStep::listChanges() const {
    if (!_changeList) {
        throw PipelineException("ChangeStep: List changes requires an allocated column of changes");
    }

    _sysMan->getChangeManager().listChanges(_changeList->getRaw());
}
