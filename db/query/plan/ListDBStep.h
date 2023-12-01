#pragma once

#include "QueryPlanStep.h"

namespace db {

class QueryPlan;

class ListDBStep : public QueryPlanStep {
public:
    ListDBStep();
    ~ListDBStep();

    std::string getName() const override;

private:
};

}
