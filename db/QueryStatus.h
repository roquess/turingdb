#pragma once

#include <string>

#include "EnumToString.h"
#include "Time.h"

namespace db {

class QueryStatus {
public:
    enum class Status {
        OK,
        PARSE_ERROR,
        ANALYZE_ERROR,
        PLAN_ERROR,
        EXEC_ERROR,
        _SIZE
    };

    QueryStatus() = default;
    explicit QueryStatus(Status status)
        : _status(status)
    {
    }

    ~QueryStatus() = default;

    Status getStatus() const { return _status; }

    bool isOk() const { return _status == Status::OK; }

    void setError(const std::string& error) { _errorMsg = error; }
    const std::string& getError() const { return _errorMsg; }

    void setTotalTime(Milliseconds totalTime) { _totalTime = totalTime; }
    Milliseconds getTotalTime() const { return _totalTime; }

private:
    Status _status {Status::OK};
    std::string _errorMsg;
    Milliseconds _totalTime {0};
};

using StatusString = EnumToString<QueryStatus::Status>::Create<
    EnumStringPair<QueryStatus::Status::OK, "OK">,
    EnumStringPair<QueryStatus::Status::PARSE_ERROR, "Parse error">,
    EnumStringPair<QueryStatus::Status::ANALYZE_ERROR, "Analyze error">,
    EnumStringPair<QueryStatus::Status::PLAN_ERROR, "Plan error">,
    EnumStringPair<QueryStatus::Status::EXEC_ERROR, "Execution error">
>;

}
