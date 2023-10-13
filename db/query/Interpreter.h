#pragma once

#include <string>

#include "StringSpan.h"
#include "QueryStatus.h"

class Buffer;

namespace db {

class InterpreterContext;
class PullPlan;

class Interpreter {
public:
    explicit Interpreter(InterpreterContext* interpCtxt);
    ~Interpreter();

    void execQuery(StringSpan query, Buffer* outBuffer) const;

private:
    InterpreterContext* _interpCtxt {nullptr};
    const std::string headerOk =
        "HTTP/1.1 200 OK\r\n";
    const std::string emptyLine = "\r\n";
    const std::string bodyBegin = "{\"status\":\"";
    const std::string timeBeginStr = "\", \"time\": \"";
    const std::string timeEndStr = "\"";
    const std::string bodyDataBegin = ",\"data\":[";
    const std::string bodyEnd = "]}\n";

    bool writeResponse(Buffer* outBuffer, PullPlan* plan, double execTime) const;
    void handleQueryError(QueryStatus status, Buffer* outBuffer) const;
};

}
