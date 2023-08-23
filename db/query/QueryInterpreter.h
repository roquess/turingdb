#pragma once

#include <string>
#include <optional>
#include <vector>

namespace db::query {

class LogicalOperator;

class QueryInterpreter {
public:
    struct PrepareResult {
        std::optional<size_t> _qid;
        bool _success {false};
    };

    struct QueryExecution {
        QueryExecution(LogicalOperator* plan);
        ~QueryExecution();

        LogicalOperator* _plan {nullptr};
    };

    QueryInterpreter();
    ~QueryInterpreter();

    PrepareResult prepare(const std::string& query);

private:
    std::vector<QueryExecution> _queries;

    size_t registerQuery(LogicalOperator* plan);
};

}
