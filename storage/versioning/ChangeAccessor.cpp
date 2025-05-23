#include "ChangeAccessor.h"

#include "Change.h"
#include "views/GraphView.h"
#include "versioning/VersionController.h"

using namespace db;

[[nodiscard]] CommitBuilder* ChangeAccessor::getTip() const {
    return _change->_tip;
}

[[nodiscard]] auto ChangeAccessor::begin() const {
    return _change->_commits.cbegin();
}
[[nodiscard]] auto ChangeAccessor::end() const {
    return _change->_commits.cend();
}

[[nodiscard]] CommitResult<void> ChangeAccessor::commit(JobSystem& jobsystem) {
    return _change->commit(jobsystem);
}

[[nodiscard]] CommitResult<void> ChangeAccessor::rebase(JobSystem& jobsystem) {
    return _change->rebase(jobsystem);
}

[[nodiscard]] CommitResult<void> ChangeAccessor::submit(JobSystem& jobsystem) {
    return _change->submit(jobsystem);
}

[[nodiscard]] ChangeID ChangeAccessor::getID() const {
    return _change->_id;
}

[[nodiscard]] GraphView ChangeAccessor::viewGraph(CommitHash commitHash) const {
    return _change->viewGraph(commitHash);
}

[[nodiscard]] const Graph* ChangeAccessor::getGraph() const {
    return _change->_versionController->getGraph();
}

ChangeAccessor::ChangeAccessor(Change* change)
    : _lock(change->_mutex),
    _change(change)
{
}
