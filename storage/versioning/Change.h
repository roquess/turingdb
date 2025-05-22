#pragma once

#include <memory>
#include <vector>
#include <mutex>

#include "versioning/CommitData.h"
#include "versioning/CommitHash.h"
#include "versioning/CommitResult.h"
#include "versioning/ChangeID.h"

namespace db {

class VersionController;
class DataPartBuilder;
class CommitBuilder;
class Commit;
class JobSystem;
class ReadTransaction;
class ChangeManager;
class WriteTransaction;

class Change {
public:
    class Accessor {
    public:
        Accessor() = default;
        ~Accessor() = default;

        Accessor(const Accessor&) = delete;
        Accessor(Accessor&&) = default;
        Accessor& operator=(const Accessor&) = delete;
        Accessor& operator=(Accessor&&) = default;

        [[nodiscard]] CommitBuilder* getTip() const {
            return _change->_tip;
        }

        [[nodiscard]] auto begin() const { return _change->_commits.cbegin(); }
        [[nodiscard]] auto end() const { return _change->_commits.cend(); }

        [[nodiscard]] CommitResult<void> commit(JobSystem& jobsystem) {
            return _change->commit(jobsystem);
        }

        [[nodiscard]] CommitResult<void> rebase(JobSystem& jobsystem) {
            return _change->rebase(jobsystem);
        }

        [[nodiscard]] CommitResult<void> submit(JobSystem& jobsystem) {
            return _change->submit(jobsystem);
        }

        [[nodiscard]] ChangeID getID() const { return _change->_id; }

        void release() {
            _lock.unlock();
        }

    private:
        friend Change;
        Change* _change {nullptr};
        std::unique_lock<std::mutex> _lock;

        Accessor(Change* change)
            : _change(change),
            _lock(_change->_mutex)
        {
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

    [[nodiscard]] WriteTransaction openWriteTransaction();
    [[nodiscard]] ReadTransaction openReadTransaction(CommitHash hash);



    [[nodiscard]] Accessor access() { return Accessor {this}; }
    [[nodiscard]] CommitHash baseHash() const;
    [[nodiscard]] ChangeID id() const { return _id; }

private:
    mutable std::mutex _mutex;
    friend ChangeManager;
    friend VersionController;

    ChangeID _id;
    VersionController* _versionController {nullptr};
    WeakArc<const CommitData> _base;

    // Committed
    std::vector<std::unique_ptr<CommitBuilder>> _commits;
    std::unordered_map<CommitHash, size_t> _commitOffsets;

    CommitBuilder* _tip {nullptr};

    explicit Change(VersionController* versionController, ChangeID id, CommitHash base);

    [[nodiscard]] CommitResult<void> commit(JobSystem& jobsystem);
    [[nodiscard]] CommitResult<void> rebase(JobSystem& jobsystem);
    [[nodiscard]] CommitResult<void> submit(JobSystem& jobsystem);
};

}
