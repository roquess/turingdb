#pragma once

#include "ExecutionContext.h"

namespace db {

class EntityPattern;
class CommitBuilder;

class BuildDataPartsStep {
public:
    struct Tag {};

    explicit BuildDataPartsStep();
    BuildDataPartsStep(BuildDataPartsStep&& other) = default;
    ~BuildDataPartsStep();

    void prepare(ExecutionContext* ctxt);

    void reset() {
    }

    bool isFinished() const {
        return true;
    }

    void execute();

    void describe(std::string& descr) const;

private:
    CommitBuilder* _change {nullptr};
};

}
