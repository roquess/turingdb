#pragma once

#include <string>

#include "QueryPlanStep.h"

namespace db {

class SearchNodeStep : public QueryPlanStep {
public:
    SearchNodeStep();
    ~SearchNodeStep();

    std::string getName() const override;
};

}
