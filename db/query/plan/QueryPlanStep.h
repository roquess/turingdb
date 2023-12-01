#pragma once

#include <string>

namespace db {

class QueryPlanStep {
public:
    virtual ~QueryPlanStep() = default;

    virtual std::string getName() const = 0;
};

}
