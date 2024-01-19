#pragma once

#include <string>
#include <optional>
#include <vector>

class PullResponse;

namespace db::query {

class InterpreterContext;
class PullPlan;

class QueryInterpreter {
public:
    struct PrepareResult {
        std::optional<size_t> _qid;
        bool _success {false};
    };

    class QueryExecution {
    public:
        explicit QueryExecution(PullPlan* plan);
        ~QueryExecution();

        PullPlan* getPlan() const { return _plan; }

    private:
        PullPlan* _plan {nullptr};
    };

    explicit QueryInterpreter(InterpreterContext* interpCtxt);
    ~QueryInterpreter();

    PrepareResult prepare(const std::string& query);

    bool pull(PullResponse* result, size_t qid);

private:
    InterpreterContext* _interpCtxt {nullptr};
    std::vector<QueryExecution*> _queries;
};

}
