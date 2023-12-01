#include "ListDBStep.h"

#include "QueryPlan.h"

using namespace db;

ListDBStep::ListDBStep()
{
}

ListDBStep::~ListDBStep() {
}

std::string ListDBStep::getName() const {
    return "ListDBStep";
}
