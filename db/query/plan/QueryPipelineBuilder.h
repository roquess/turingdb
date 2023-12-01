#pragma once

namespace db {

class QueryPlan;

class QueryPipelineBuilder {
public:
    QueryPipelineBuilder(const QueryPlan* plan);
    ~QueryPipelineBuilder();

private:
    const QueryPlan* _plan {nullptr};
};

}