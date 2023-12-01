#pragma once

#include <memory>

namespace db {

class QueryCommand;
class InterpreterContext;
class QueryPlan;
class QueryPlanStep;
class ListCommand;
class OpenCommand;
class SelectCommand;

class Planner {
public:
    Planner(const QueryCommand* query,
            InterpreterContext* interpCtxt);
    ~Planner();

    QueryPlan* getQueryPlan() const { return _queryPlan.get(); }

    bool buildQueryPlan();

private:
    const QueryCommand* _query {nullptr};
    InterpreterContext* _interpCtxt {nullptr};
    std::unique_ptr<QueryPlan> _queryPlan;

    bool planListCommand(const ListCommand* cmd);
    bool planOpenCommand(const OpenCommand* cmd);
    bool planSelectCommand(const SelectCommand* cmd);
    QueryPlanStep* planPathPattern(const PathPattern* pattern);
};

}
