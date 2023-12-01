#include "QueryPipelineBuilder.h"

using namespace db;

QueryPipelineBuilder::QueryPipelineBuilder(const QueryPlan* plan)
    : _plan(plan)
{
}

QueryPipelineBuilder::~QueryPipelineBuilder() {
}