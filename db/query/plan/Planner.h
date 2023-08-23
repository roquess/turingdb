#pragma once

#include "PullPlan.h"

namespace db::query {

class QueryCommand;

class Planner {
public:
    Planner();
    ~Planner();

    std::unique_ptr<PullPlan> makePlan(const QueryCommand* cmd);

private:
    using PullPlanPtr = std::unique_ptr<PullPlan>;

    PullPlanPtr planListCommand(const ListCommand* cmd);
    PullPlanPtr planListDatabases(const ListCommand* cmd);
};

}
