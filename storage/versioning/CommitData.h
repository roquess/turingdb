#pragma once

#include "versioning/CommitHash.h"
#include "versioning/CommitHistory.h"

namespace db {

class DataPart;
class CommitBuilder;
class CommitLoader;
class GraphLoader;
class GraphMetadata;
class VersionController;

class CommitData {
public:
    explicit CommitData(CommitHash hash)
        : _hash(hash)
    {
    }

    ~CommitData() = default;

    CommitData(const CommitData&) = delete;
    CommitData(CommitData&&) = delete;
    CommitData& operator=(const CommitData&) = delete;
    CommitData& operator=(CommitData&&) = delete;

    [[nodiscard]] DataPartSpan allDataparts() const { return _history.allDataparts(); }
    [[nodiscard]] DataPartSpan commitDataparts() const { return _history.commitDataparts(); }
    [[nodiscard]] GraphMetadata& metadata() const { return *_graphMetadata; }
    [[nodiscard]] std::span<const CommitView> commits() const { return _history.commits(); }
    [[nodiscard]] const CommitHistory& history() const { return _history; }
    [[nodiscard]] CommitHistory& history() { return _history; }

private:
    friend CommitBuilder;
    friend CommitLoader;
    friend GraphLoader;
    friend VersionController;

    CommitHash _hash;
    CommitHistory _history;
    GraphMetadata* _graphMetadata {nullptr};
};

}

