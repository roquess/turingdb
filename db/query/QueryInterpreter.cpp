#include "QueryInterpreter.h"

#include "QueryParser.h"

#include "plan/Planner.h"
#include "plan/PullPlan.h"

#include "DBService.grpc.pb.h"

using namespace db::query;

// QueryExecution
QueryInterpreter::QueryExecution::QueryExecution(PullPlan* plan)
    : _plan(plan)
{
}

QueryInterpreter::QueryExecution::~QueryExecution() {
}

// QueryInterpreter
QueryInterpreter::QueryInterpreter(InterpreterContext* interpCtxt)
    : _interpCtxt(interpCtxt)
{
}

QueryInterpreter::~QueryInterpreter() {
    for (QueryExecution* queryExec : _queries) {
        delete queryExec;
    }
}

QueryInterpreter::PrepareResult QueryInterpreter::prepare(const std::string& query) {
    QueryParser parser;
    QueryCommand* cmd = parser.parse(query);
    if (!cmd) {
        return PrepareResult();
    }

    Planner planner(_interpCtxt);
    PullPlan* plan = planner.makePlan(cmd);
    if (!plan) {
        return PrepareResult();
    }

    const size_t qid = _queries.size();
    _queries.push_back(new QueryExecution(plan));

    PrepareResult res;
    res._success = true;
    res._qid = qid;
    return res;
}

bool QueryInterpreter::pull(PullResponse* pullRes, size_t qid) {
    if (qid >= _queries.size()) {
        return false;
    }

    QueryExecution* queryExec = _queries[qid];
    PullPlan* pullPlan = queryExec->getPlan();
    if (!pullPlan) {
        return false;
    }

    return pullPlan->pull(pullRes);
}
