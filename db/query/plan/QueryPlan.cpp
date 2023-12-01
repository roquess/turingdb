#include "QueryPlan.h"

#include "QueryPlanStep.h"

using namespace db;

QueryPlan::QueryPlan()
{
}

QueryPlan::~QueryPlan() {
    for (QueryPlanStep* step : _steps) {
        delete step;
    }
}

void QueryPlan::addStep(QueryPlanStep* step) {
    _steps.push_back(step);
}
