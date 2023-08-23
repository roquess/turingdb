#pragma once

#include <memory>

#include "LogicalOperator.h"
#include "SymbolTable.h"

namespace db::query {

class PullPlan {
public:
    PullPlan(std::unique_ptr<LogicalOperator> plan,
             std::unique_ptr<SymbolTable> symTable);
    ~PullPlan();

private:
    std::unique_ptr<LogicalPlan> _plan;
    std::unique_ptr<SymbolTable> _symTable;
};

}
