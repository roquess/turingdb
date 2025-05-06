#include <mutex>
#include <shared_mutex>

#include "ChangeManager.h"
#include "JobSystem.h"
#include "Profiler.h"
#include "versioning/CommitBuilder.h"

using namespace db;

ChangeManager::ChangeManager() = default;

ChangeManager::~ChangeManager() = default;

CommitHash ChangeManager::storeChange(std::unique_ptr<CommitBuilder> builder) {
    std::unique_lock guard(_changesLock);
    const auto hash = builder->hash();
    _changes.emplace(hash, std::move(builder));

    return hash;
}

ChangeResult<CommitBuilder*> ChangeManager::getChange(CommitHash changeHash) {
    std::shared_lock guard(_changesLock);

    const auto it = _changes.find(changeHash);
    if (it == _changes.end()) {
        return ChangeError::result(ChangeErrorType::CHANGE_NOT_EXISTS);
    }

    return it->second.get();
}

ChangeResult<void> ChangeManager::acceptChange(CommitHash changeHash, JobSystem& jobsystem) {
    std::unique_lock guard(_changesLock);

    const auto it = _changes.find(changeHash);
    if (it == _changes.end()) {
        return ChangeError::result(ChangeErrorType::CHANGE_NOT_EXISTS);
    }

    std::unique_ptr<CommitBuilder> builder = std::move(it->second);
    _changes.erase(it);

    if (auto res = builder->rebaseAndCommit(jobsystem); !res) {
        return ChangeError::result(ChangeErrorType::COULD_NOT_ACCEPT_CHANGE, res.error());
    }

    return {};
}

ChangeResult<void> ChangeManager::deleteChange(CommitHash changeHash) {
    std::unique_lock guard(_changesLock);

    const auto it = _changes.find(changeHash);
    if (it == _changes.end()) {
        return ChangeError::result(ChangeErrorType::CHANGE_NOT_EXISTS);
    }

    _changes.erase(it);

    return {};
}

void ChangeManager::listChanges(std::vector<const CommitBuilder*>& list) const {
    std::shared_lock guard(_changesLock);

    list.clear();
    for (const auto& [hash, builder] : _changes) {
        list.emplace_back(builder.get());
    }
}
