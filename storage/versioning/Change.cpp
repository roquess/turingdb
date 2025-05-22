#include "Change.h"

#include "Panic.h"
#include "versioning/CommitBuilder.h"
#include "versioning/DataPartRebaser.h"
#include "versioning/MetadataRebaser.h"
#include "versioning/VersionController.h"
#include "versioning/Transaction.h"

using namespace db;

Change::~Change() = default;

Change::Change(VersionController* versionController, ChangeID id, CommitHash base)
    : _id(id),
      _versionController(versionController),
      _base(versionController->openReadTransaction(base).commitData()) {
    auto tip = CommitBuilder::prepare(*_versionController,
                                      this,
                                      GraphView {*_base});
    _tip = tip.get();
    _commitOffsets.emplace(_tip->hash(), _commits.size());
    _commits.emplace_back(std::move(tip));
}

std::unique_ptr<Change> Change::create(VersionController* versionController,
                                       ChangeID id,
                                       CommitHash base) {
    auto* ptr = new Change(versionController, id, base);

    return std::unique_ptr<Change> {ptr};
}

WriteTransaction Change::openWriteTransaction() {
    return WriteTransaction {
        _tip->openReadTransaction().commitData(),
        _tip,
        &_tip->getCurrentBuilder(),
        this->access()};
}

ReadTransaction Change::openReadTransaction(CommitHash hash) {
    auto commit = _commitOffsets.find(hash);
    if (commit == _commitOffsets.end()) {
        return ReadTransaction {}; // Invalid
    }

    return _commits[commit->second]->openReadTransaction();
}

CommitResult<void> Change::commit(JobSystem& jobsystem) {
    Profile profile {"Change::commit"};

    if (auto res = _tip->buildAllPending(jobsystem); !res) {
        return res;
    }

    auto newTip = CommitBuilder::prepare(*_versionController,
                                         this,
                                         _commits.back()->openReadTransaction().viewGraph());
    _tip = newTip.get();
    _commitOffsets.emplace(_tip->hash(), _commits.size());
    _commits.emplace_back(std::move(newTip));

    return {};
}

CommitResult<void> Change::rebase(JobSystem& jobsystem) {
    Profile profile {"Change::rebase"};

    _base = _versionController->openReadTransaction().commitData();

    MetadataRebaser metadataRebaser;
    DataPartRebaser dataPartRebaser;

    const auto* prevCommitData = _base.get();
    const auto* prevHistory = &_base->history();

    for (auto& commitBuilder : _commits) {
        // Rebasing means:
        // 1. Rebase the metadata
        // 2. Get all commits/dataparts from the previous commit history
        // 3. Add back dataparts of current commit and rebase them
        auto& data = commitBuilder->commitData();
        auto& history = data.history();
        history.rebase(*prevHistory);

        // TODO Ideally, commitDataparts should be a span
        //      and we should:
        //          CommitHistory newHistory = CommitHistory::fromPreviousCommit(previousHistory);
        //          newHistory.pushDataparts(commitBuilder->commitDataparts());

        metadataRebaser.clear();
        metadataRebaser.rebase(prevCommitData->metadata(), commitBuilder->metadata());

        const auto& prevDataParts = prevHistory->_allDataparts;

        if (!prevDataParts.empty()) {
            const auto* prevPart = prevDataParts.back().get();
            for (auto& part : history._commitDataparts) {
                dataPartRebaser.rebase(metadataRebaser, *prevPart, *part);
                prevPart = part.get();
            }
        }

        prevCommitData = &commitBuilder->commitData();
        prevHistory = &prevCommitData->history();
    }

    return {};
}

CommitHash Change::baseHash() const {
    return _base->hash();
}

CommitResult<void> Change::submit(JobSystem& jobsystem) {
    Profile profile {"Change::submit"};

    if (auto res = _tip->buildAllPending(jobsystem); !res) {
        return res;
    }

    if (auto res = _versionController->submitChange(this, jobsystem); !res) {
        return res;
    }

    return {};
}

