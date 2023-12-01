#pragma once

#include <string>

#include "QueryPlanStep.h"

namespace db {

class OpenDBStep : public QueryPlanStep {
public:
    OpenDBStep(const std::string& path);
    ~OpenDBStep();

    std::string getName() const override;

private:
    std::string _dbPath;
};

}
