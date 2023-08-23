#include "PullPlan.h"

using namespace db::query;

PullPlan::PullPlan(std::unique_ptr<LogicalOperator> plan,
                   std::unique_ptr<SymbolTable> symTable)
    : _plan(plan),
    _symTable(symTable)
{
}

PullPlan::~PullPlan() {
}
