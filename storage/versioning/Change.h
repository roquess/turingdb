#pragma once

#include <memory>
#include <vector>
#include <mutex>

#include "versioning/CommitHash.h"
#include "versioning/CommitResult.h"
#include "versioning/Transaction.h"
#include "versioning/ChangeID.h"

namespace db {

class VersionController;
class DataPartBuilder;
class CommitBuilder;
class Commit;
class JobSystem;

class Change {
public:
    class Accessor {
    public:
        ~Accessor() = default;

        Accessor(const Accessor&) = delete;
        Accessor(Accessor&&) = delete;
        Accessor& operator=(const Accessor&) = delete;
        Accessor& operator=(Accessor&&) = delete;

        [[nodiscard]] size_t commitCount() const { return _change->_commits.size(); }
        [[nodiscard]] CommitBuilder* getPendingCommit(CommitHash hash) const {
            auto it = _change->_commitBuilders.find(hash);
            if (it == _change->_commitBuilders.end()) {
                return nullptr;
            }

            return it->second.get();
        }

        [[nodiscard]] Commit* getCommit(CommitHash hash) const {
            if (_change->_commits.empty()) {
                return nullptr;
            }

            if (hash == CommitHash::head()) {
                return _change->_commits.back().get();
            }

            auto it = _change->_commitOffsets.find(hash);
            if (it == _change->_commitOffsets.end()) {
                return nullptr;
            }

            return _change->_commits[it->second].get();
        }

        [[nodiscard]] auto begin() const { return _change->_commits.cbegin(); }
        [[nodiscard]] auto end() const { return _change->_commits.cend(); }


    private:
        friend Change;
        const Change* _change {nullptr};
        std::scoped_lock<std::mutex> _lock;

        Accessor(const Change* change)
            : _change(change),
              _lock(_change->_mutex) {
        }
    };

    ~Change();

    Change(const Change&) = delete;
    Change(Change&&) = delete;
    Change& operator=(const Change&) = delete;
    Change& operator=(Change&&) = delete;

    [[nodiscard]] static std::unique_ptr<Change> create(VersionController* versionController,
                                                        ChangeID id,
                                                        CommitHash base);

    [[nodiscard]] Transaction openTransaction(CommitHash hash = CommitHash::head()) const;


    CommitBuilder* newCommit();
    [[nodiscard]] CommitResult<void> commit(CommitHash hash, JobSystem& jobsystem);
    [[nodiscard]] CommitResult<void> rebase(CommitHash hash, JobSystem& jobsystem);
    [[nodiscard]] CommitResult<void> commitAllPending(JobSystem& jobsystem);

    [[nodiscard]] Accessor access() const { return Accessor {this}; }
    [[nodiscard]] CommitHash baseHash() const;
    [[nodiscard]] ChangeID id() const { return _id; }

private:
    mutable std::mutex _mutex;

    ChangeID _id;
    VersionController* _versionController {nullptr};
    Transaction _base;

    // Committed
    std::vector<std::unique_ptr<Commit>> _commits;
    std::unordered_map<CommitHash, size_t> _commitOffsets;

    // Pending
    std::unordered_map<CommitHash, std::unique_ptr<CommitBuilder>> _commitBuilders;

    explicit Change(VersionController* versionController, ChangeID id, CommitHash base);

    CommitResult<void> commit(std::unique_ptr<CommitBuilder> commitBuilder, JobSystem& jobsystem);
};

}
