#pragma once

#include <unordered_map>

#include "RWSpinLock.h"
#include "versioning/ChangeResult.h"
#include "versioning/CommitHash.h"
#include "versioning/Change.h"

namespace db {

class Graph;

class JobSystem;

class ChangeManager {
public:
    struct GraphChangePair {
        std::unique_ptr<Change> _change;
        Graph* _graph {nullptr};
    };

    ChangeManager();
    ~ChangeManager();

    ChangeManager(const ChangeManager&) = delete;
    ChangeManager(ChangeManager&&) = delete;
    ChangeManager& operator=(const ChangeManager&) = delete;
    ChangeManager& operator=(ChangeManager&&) = delete;

    CommitHash storeChange(std::unique_ptr<Change> change);
    ChangeResult<Change*> getChange(ChangeID changeHash);
    ChangeResult<void> acceptChange(ChangeID changeHash, JobSystem&);
    ChangeResult<void> deleteChange(ChangeID changeHash);

    void listChanges(std::vector<const Change*>& list) const;

private:
    mutable RWSpinLock _changesLock;
    std::unordered_map<ChangeID, GraphChangePair> _changes;
};

}
