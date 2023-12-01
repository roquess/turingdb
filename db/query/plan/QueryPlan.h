#pragma once

#include <vector>

namespace db {

class QueryPlanStep;

class QueryPlan {
public:
    using QueryPlanSteps = std::vector<QueryPlanStep*>;

    QueryPlan();
    ~QueryPlan();

    const QueryPlanSteps& steps() const { return _steps; }

    void addStep(QueryPlanStep* step);

private:
    QueryPlanSteps _steps;
};

}
