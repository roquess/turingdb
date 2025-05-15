#pragma once

#include <string>

#include <variant>

#include "ExecutionContext.h"
#include "columns/ColumnVector.h"
#include "views/GraphView.h"
#include "versioning/CommitResult.h"
#include "versioning/CommitHash.h"

namespace db {

class ExecutionContext;
class SystemManager;
using CommitInfo = std::variant<CommitHash, std::string>;

class CommitStep {
public:
    struct Tag {};

    CommitStep();
    ~CommitStep();

    void prepare(ExecutionContext* ctxt);

    inline bool isFinished() const { return true; }

    void reset() {}

    void execute();

    void describe(std::string& descr) const;

private:
    JobSystem* _jobSystem {nullptr};
    CommitBuilder* _commitBuilder {nullptr};

    CommitResult<CommitHash> createCommit() const;
    CommitResult<void> acceptCommit() const;
    CommitResult<void> deleteCommit() const;
    void listCommits() const;
};

}
