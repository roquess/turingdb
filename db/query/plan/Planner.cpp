#include "Planner.h"

#include <vector>

#include "QueryCommand.h"
#include "QueryPlan.h"
#include "FromTarget.h"
#include "PathPattern.h"

#include "ListDBStep.h"
#include "OpenDBStep.h"

using namespace db;

Planner::Planner(const QueryCommand* query,
                 InterpreterContext* interpCtxt)
    : _query(query),
    _interpCtxt(interpCtxt),
    _queryPlan(std::make_unique<QueryPlan>())
{
}

Planner::~Planner() {
}

bool Planner::buildQueryPlan() {
    switch (_query->getKind()) {
        case QueryCommand::QCOM_LIST_COMMAND:
            return planListCommand(static_cast<const ListCommand*>(_query));
        break;

        case QueryCommand::QCOM_OPEN_COMMAND:
            return planOpenCommand(static_cast<const OpenCommand*>(_query));
        break;

        case QueryCommand::QCOM_SELECT_COMMAND:
            return planSelectCommand(static_cast<const SelectCommand*>(_query));
        break;

        default:
            return false;
        break;
    }
}

bool Planner::planListCommand(const ListCommand* cmd) {
    ListDBStep* listDB = new ListDBStep();
    _queryPlan->addStep(listDB);
    return true;
}

bool Planner::planOpenCommand(const OpenCommand* cmd) {
    OpenDBStep* openDB = new OpenDBStep(cmd->getPath());
    _queryPlan->addStep(openDB);
    return true;
}

bool Planner::planSelectCommand(const SelectCommand* cmd) {
    // Plan for each FROM target
    std::vector<QueryPlanStep*> targetsSteps;
    for (const FromTarget* fromTarget : cmd->fromTargets()) {
        QueryPlanStep* patternStep = planPathPattern(fromTarget->getPattern());
        if (!patternStep) {
            return nullptr;
        }

        targetsSteps.push_back(patternStep);
    }

    return false;
}

QueryPlanStep* Planner::planPathPattern(const PathPattern* pattern) {
    for (const PathElement* pathElement : pattern->elements()) {
    }
}
