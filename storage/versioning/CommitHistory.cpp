#include "CommitHistory.h"

#include "CommitView.h"

using namespace db;

CommitHistory::CommitHistory() = default;

CommitHistory::~CommitHistory() = default;

std::span<const CommitView> CommitHistory::commits() const
{
    return std::span<const CommitView>(_commits);
}


void CommitHistory::rebase(const CommitHistory& base) {
    // Commits
    const CommitView tip = _commits.back();
    _commits = base._commits;
    _commits.push_back(tip);

    // Dataparts
    _allDataparts = base._allDataparts;
    _allDataparts.resize(_allDataparts.size() + _commitDataparts.size());
    std::copy(_commitDataparts.begin(),
            _commitDataparts.end(),
            _allDataparts.begin() + base._allDataparts.size());
} 

void CommitHistory::newFromPrevious(const CommitHistory& base) {
    _commits = base._commits;
    _allDataparts = base._allDataparts;
    _commitDataparts.clear();
}
