#pragma once

#include <variant>

#include "Match.h"

namespace db {

// TODOl: Add support for Unwind and Call
using ReadingStatement = std::variant<Match*>;

template <typename T>
concept IsReadingStatement = std::is_same_v<T, Match>;

class ReadingStatementContainer {
public:
    ReadingStatementContainer() = default;
    ~ReadingStatementContainer() = default;

    static std::unique_ptr<ReadingStatementContainer> create() {
        return std::make_unique<ReadingStatementContainer>();
    }

    ReadingStatementContainer(const ReadingStatementContainer&) = delete;
    ReadingStatementContainer& operator=(const ReadingStatementContainer&) = delete;
    ReadingStatementContainer(ReadingStatementContainer&&) = delete;
    ReadingStatementContainer& operator=(ReadingStatementContainer&&) = delete;

    void add(const ReadingStatement& statement) {
        _statements.emplace_back(statement);
    }

private:
    std::vector<ReadingStatement> _statements;
};

}
