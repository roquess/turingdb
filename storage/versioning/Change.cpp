#include "Change.h"

#include "reader/GraphReader.h"
#include "versioning/CommitBuilder.h"
#include "versioning/DataPartRebaser.h"
#include "versioning/MetadataRebaser.h"
#include "versioning/VersionController.h"

#include <mutex>

using namespace db;

Change::~Change() = default;

Change::Change(VersionController* versionController, ChangeID id, CommitHash base)
    : _id(id),
      _versionController(versionController),
      _base(versionController->openTransaction(base)) {
}

std::unique_ptr<Change> Change::create(VersionController* versionController,
                                       ChangeID id,
                                       CommitHash base) {
    auto* ptr = new Change(versionController, id, base);

    return std::unique_ptr<Change> {ptr};
}

Transaction Change::openTransaction(CommitHash hash) const {
    std::scoped_lock lock(_mutex);

    if (hash == CommitHash::head()) {
        if (_commits.empty()) {
            return _base;
        }

        return _commits.back()->openTransaction();
    }

    auto it = _commitOffsets.find(hash);
    if (it == _commitOffsets.end()) {
        return Transaction {};
    }

    return _commits[it->second]->openTransaction();
}

CommitBuilder* Change::newCommit() {
    std::scoped_lock lock(_mutex);

    GraphView tipView = _commits.empty()
                          ? _base.viewGraph()
                          : _commits.back()->openTransaction().viewGraph();

    auto commitBuilder = CommitBuilder::prepare(*_versionController, this, tipView);
    auto* ptr = commitBuilder.get();

    _commitBuilders[commitBuilder->hash()] = std::move(commitBuilder);

    return ptr;
}

CommitResult<void> Change::commit(CommitHash hash, JobSystem& jobsystem) {
    Profile profile {"Change::commit"};

    std::scoped_lock lock(_mutex);

    auto it = _commitBuilders.find(hash);
    if (it == _commitBuilders.end()) {
        return CommitError::result(CommitErrorType::COMMIT_HASH_NOT_EXISTS);
    }

    if (_commitOffsets.contains(hash)) {
        return CommitError::result(CommitErrorType::COMMIT_HASH_EXISTS);
    }

    auto& commitBuilder = it->second;
    this->commit(std::move(commitBuilder), jobsystem);

    return {};
}

CommitResult<void> Change::rebase(CommitHash hash, JobSystem& jobsystem) {
    Profile profile {"Change::rebase"};

    std::scoped_lock lock {_mutex};

    /* The transaction does not need to be kept alive if the commit
     * is owned by the change */

    const auto& tipData = _commits.empty()
                            ? _base.commitData()
                            : _commits.back()->openTransaction().commitData();

    const auto& tipHistory = tipData.history();
    const auto& tipMetadata = tipData.metadata();
    const auto& tipCommits = tipHistory.commits();

    size_t commitIndex = 0;

    auto it = _commitBuilders.find(hash);
    if (it == _commitBuilders.end()) {
        return CommitError::result(CommitErrorType::COMMIT_HASH_NOT_EXISTS);
    }

    if (_commitOffsets.contains(hash)) {
        return CommitError::result(CommitErrorType::COMMIT_HASH_EXISTS);
    }

    auto& commitBuilder = it->second;
    if (auto res = commitBuilder->buildAllPending(jobsystem); !res) {
        return res;
    }

    MetadataRebaser metadataRebaser;
    metadataRebaser.rebase(tipMetadata, commitBuilder->metadata());
    auto& commit = commitBuilder->_commit;

    size_t partIndex = 0;

    auto& currentCommits = commit->history()._commits;

    for (const auto& c : tipCommits) {
        if (c != currentCommits[commitIndex]) {
            currentCommits.insert(currentCommits.begin() + commitIndex, c);
        }
        commitIndex++;
    }

    auto& currentHistory = commit->history();
    auto& currentDataparts = currentHistory._allDataparts;

    for (const auto& p : tipHistory.allDataparts()) {
        if (p != currentHistory.allDataparts()[partIndex]) {
            currentDataparts.insert(currentDataparts.begin() + partIndex, p);
        }
        partIndex++;
    }

    DataPartRebaser partRebaser;

    if (tipData.allDataparts().empty()) {
        return {};
    }

    const DataPart* prevPart = tipData.allDataparts().back().get();
    for (auto& part : currentHistory._commitDataparts) {
        partRebaser.rebase(metadataRebaser, *prevPart, *part);
        prevPart = part.get();
    }

    return {};
}

CommitResult<void> Change::commitAllPending(JobSystem& jobsystem) {
    Profile profile {"Change::commitAllPending"};

    std::scoped_lock lock(_mutex);

    GraphView tipView = _commits.empty()
                          ? _base.viewGraph()
                          : _commits.back()->openTransaction().viewGraph();

    for (auto& [hash, commitBuilder] : _commitBuilders) {
        tipView = _commits.back()->openTransaction().viewGraph();

        const auto reader = tipView.read();
        const auto firstNodeID = reader.getNodeCount();
        const auto firstEdgeID = reader.getEdgeCount();
        commitBuilder->setEntityIDs(firstNodeID, firstEdgeID);
        if (auto res = commitBuilder->buildAllPending(jobsystem); !res) {
            return res;
        }

        this->commit(std::move(commitBuilder), jobsystem);
    }

    _commitBuilders.clear();

    return {};
}

CommitHash Change::baseHash() const {
    return _base.commitData().hash();
}


CommitResult<void> Change::commit(std::unique_ptr<CommitBuilder> commitBuilder, JobSystem& jobsystem) {
    if (auto res = commitBuilder->buildAllPending(jobsystem); !res) {
        return res;
    }

    const GraphView currentView = commitBuilder->viewGraph();

    /* The transaction does not need to be kept alive if the commit
     * is owned by the change */

    const GraphView tipView = _commits.empty()
                                ? _base.viewGraph()
                                : _commits.back()->openTransaction().viewGraph();


    // Checking if the commit is up to date with the tip
    const std::span commitDataparts = currentView.dataparts();
    const std::span tipDataparts = tipView.dataparts();

    for (size_t i = 0; i < tipDataparts.size(); i++) {
        if (commitDataparts[i].get() != tipDataparts[i].get()) {
            return CommitError::result(CommitErrorType::COMMIT_NEEDS_REBASE);
        }
    }


    auto commit = commitBuilder->build(jobsystem);
    if (!commit) {
        return commit.get_unexpected();
    }

    _commitOffsets.emplace(commit.value()->hash(), _commits.size());
    _commits.emplace_back(std::move(commit.value()));

    return {};
}
