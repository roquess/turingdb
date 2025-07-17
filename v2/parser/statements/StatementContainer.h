#pragma once

#include <memory>
#include <vector>

namespace db {

class Statement;

class StatementContainer {
public:
    StatementContainer() = default;
    ~StatementContainer() = default;

    static std::unique_ptr<StatementContainer> create() {
        return std::make_unique<StatementContainer>();
    }

    StatementContainer(const StatementContainer&) = delete;
    StatementContainer& operator=(const StatementContainer&) = delete;
    StatementContainer(StatementContainer&&) = delete;
    StatementContainer& operator=(StatementContainer&&) = delete;

    void add(const Statement* statement) {
        _statements.push_back(statement);
    }

private:
    std::vector<const Statement*> _statements;
};

}
