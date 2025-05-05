#pragma once

#include <unordered_map>

#include "RWSpinLock.h"
#include "columns/ColumnVector.h"
#include "versioning/ChangeResult.h"
#include "versioning/CommitHash.h"

namespace db {

class CommitBuilder;
class JobSystem;

class ChangeManager {
public:
    ChangeManager();
    ~ChangeManager();

    ChangeManager(const ChangeManager&) = delete;
    ChangeManager(ChangeManager&&) = delete;
    ChangeManager& operator=(const ChangeManager&) = delete;
    ChangeManager& operator=(ChangeManager&&) = delete;

    CommitHash storeChange(std::unique_ptr<CommitBuilder> builder);
    ChangeResult<CommitBuilder*> getChange(CommitHash changeHash);
    ChangeResult<void> acceptChange(CommitHash changeHash, JobSystem&);
    ChangeResult<void> deleteChange(CommitHash changeHash);

    void listChanges(std::vector<const CommitBuilder*>& list) const;

private:
    mutable RWSpinLock _changesLock;
    std::unordered_map<CommitHash, std::unique_ptr<CommitBuilder>> _changes;
};

}
