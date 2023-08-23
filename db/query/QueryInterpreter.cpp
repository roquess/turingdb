#include "QueryInterpreter.h"

#include "QueryParser.h"

#include "plan/Planner.h"
#include "plan/LogicalOperator.h"

using namespace db::query;

// QueryExecution
QueryInterpreter::QueryExecution::QueryExecution(LogicalOperator* plan)
    : _plan(plan)
{
}

QueryInterpreter::QueryExecution::~QueryExecution() {
    if (_plan) {
        delete _plan;
    }
}

// QueryInterpreter
QueryInterpreter::QueryInterpreter()
{
}

QueryInterpreter::~QueryInterpreter() {
}

QueryInterpreter::PrepareResult QueryInterpreter::prepare(const std::string& query) {
    QueryParser parser;
    QueryCommand* cmd = parser.parse(query);
    if (!cmd) {
        return PrepareResult();
    }

    Planner planner;
    LogicalOperator* plan = planner.makePlan(cmd);
    if (!plan) {
        return PrepareResult();
    }

    const size_t qid = registerQuery(plan);

    PrepareResult res;
    res._success = true;
    res._qid = qid;
    return res;
}

size_t QueryInterpreter::registerQuery(LogicalOperator* plan) {
    const size_t qid = _queries.size();

    _queries.emplace_back(plan);

    return qid;
}
