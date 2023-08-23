#include "Planner.h"

#include "QueryCommand.h"
#include "LogicalOperator.h"
#include "Value.h"
#include "SymbolTable.h"

using namespace db::query;
using namespace db;

Planner::Planner()
{
}

Planner::~Planner() {
}

PullPlanPtr Planner::makePlan(const QueryCommand* cmd) {
    switch (cmd->getKind()) {
        case QueryCommand::QCOM_LIST_COMMAND:
            return planListCommand(static_cast<const ListCommand*>(cmd));
    }
    return nullptr;
}

PullPlanPtr Planner::planListCommand(const ListCommand* cmd) {
    switch (cmd->getSubType()) {
        case ListCommand::LCOM_DATABASES:
            return planListDatabases(cmd);
    }

    return nullptr;
}

PullPlanPtr Planner::planListDatabases(const ListCommand* cmd) {
    const auto callback = [](Frame& frame, ExecutionContext* ctxt, std::vector<std::vector<Value>>& rows) {
    };

    auto symTable = std::make_unique<SymbolTable>();
    std::vector<Symbol*> outputSymbols;
    outputSymbols.emplace_back(symTable.createSymbol("name"));

    OutputTableOperator* plan = new OutputTableOperator(outputSymbols, callback);
    return std::make_unique<PullPlan>(std::unique_ptr<OutputTableOperator>(plan),
                                      symTable);
}
