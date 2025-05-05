#pragma once

#include <string>

#include <variant>

#include "ExecutionContext.h"
#include "columns/ColumnVector.h"
#include "views/GraphView.h"
#include "versioning/ChangeResult.h"
#include "versioning/CommitHash.h"
#include "ChangeOpType.h"

namespace db {

class ExecutionContext;
class SystemManager;
using ChangeInfo = std::variant<CommitHash, std::string>;

class ChangeStep {
public:
    struct Tag {};

    ChangeStep(ChangeOpType type,
               ColumnVector<const CommitBuilder*>* output);
    ~ChangeStep();

    void prepare(ExecutionContext* ctxt);

    inline bool isFinished() const { return true; }

    void reset() {}

    void execute();

    void describe(std::string& descr) const;

private:
    SystemManager* _sysMan {nullptr};
    JobSystem* _jobSystem {nullptr};
    GraphView _view;
    ChangeOpType _type {};
    ChangeInfo _changeInfo;
    ColumnVector<const CommitBuilder*>* _output {nullptr};

    ChangeResult<CommitHash> createChange() const;
    ChangeResult<void> acceptChange() const;
    ChangeResult<void> deleteChange() const;
    void listChanges() const;
};

}
