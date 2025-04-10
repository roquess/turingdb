#pragma once

#include <vector>

#include "ArcManager.h"
#include "DataPartSpan.h"
#include "versioning/CommitView.h"

namespace db {

class DataPart;
class CommitBuilder;
class CommitLoader;
class GraphLoader;
class VersionController;
class CommitView;

class CommitHistory {
public:
    CommitHistory();
    ~CommitHistory();

    CommitHistory(const CommitHistory&) = delete;
    CommitHistory& operator=(const CommitHistory&) = delete;
    CommitHistory(CommitHistory&&) = delete;
    CommitHistory& operator=(CommitHistory&&) = delete;

    DataPartSpan allDataparts() const { return _allDataparts; }
    DataPartSpan commitDataparts() const { return _commitDataparts; }
    DataPartSpan commitDataparts() { return _commitDataparts; }
    std::span<const CommitView> commits() const;

    void pushPreviousDataparts(DataPartSpan dataparts) {
        const size_t prevSize = _allDataparts.size();
        _allDataparts.resize(prevSize + dataparts.size());
        std::copy(dataparts.begin(), dataparts.end(), _allDataparts.begin() + prevSize);
    }

    void pushCommitDatapart(WeakArc<DataPart>& datapart) {
        _commitDataparts.push_back(datapart);
    }

    void pushPreviousCommits(std::span<const CommitView> commits) {
        const size_t prevSize = _commits.size();
        _commits.resize(prevSize + commits.size());
        std::copy(commits.begin(), commits.end(), _commits.begin() + prevSize);
    }

    void pushCommit(const CommitView& commit) {
        _commits.push_back(commit);
    }

private:
    friend CommitBuilder;
    friend CommitLoader;
    friend GraphLoader;
    friend VersionController;

    /// Stores all the data parts that are part of the commit history.
    std::vector<WeakArc<DataPart>> _allDataparts;

    /// Stores the data parts that belong to the last commit.
    std::vector<WeakArc<DataPart>> _commitDataparts;

    /// Stores the whole history up to (including) this commit.
    std::vector<CommitView> _commits;
};

}
