#pragma once

#include <memory>

#include "Query.h"

namespace db {

class Return;
class ReadingStatementContainer;

class SinglePartQuery : public Query {
public:
    SinglePartQuery() = default;
    ~SinglePartQuery() override = default;

    SinglePartQuery(const SinglePartQuery&) = delete;
    SinglePartQuery(SinglePartQuery&&) = delete;
    SinglePartQuery& operator=(const SinglePartQuery&) = delete;
    SinglePartQuery& operator=(SinglePartQuery&&) = delete;

    static std::unique_ptr<SinglePartQuery> create() {
        return std::make_unique<SinglePartQuery>();
    }

    bool hasReturn() const {
        return _return != nullptr;
    }

    bool hasReadingStatements() const {
        return _readingStatements != nullptr;
    }

    const Return& returnStatement() const {
        return *_return;
    }

    const ReadingStatementContainer& readingStatements() const {
        return *_readingStatements;
    }

    void setReturn(Return* returnStatement) {
        _return = returnStatement;
    }

    void setReadingStatements(ReadingStatementContainer* readingStatements) {
        _readingStatements = readingStatements;
    }

private:
    Return* _return {nullptr};
    ReadingStatementContainer* _readingStatements {nullptr};
};


}
