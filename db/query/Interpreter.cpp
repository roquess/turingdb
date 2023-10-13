#include "Interpreter.h"

#include <string_view>
#include <chrono>

#include "QueryParser.h"
#include "plan/Planner.h"
#include "plan/PullPlan.h"
#include "SymbolTable.h"

#include "Buffer.h"

using namespace db;

using Clock = std::chrono::system_clock;

namespace {

static const std::string Ok = "OK";
static const std::string ParseError = "PARSE_ERROR";
static const std::string PlanError = "PLAN_ERROR";
static const std::string ExecError = "EXEC_ERROR";
static const std::string UnknownError = "UNKNOWN_ERROR";
static const std::string BufferOverflow = "BUFFER_OVERFLOW";

const std::string& getQueryStatusString(QueryStatus status) {
    switch (status) {
        case QueryStatus::OK:
            return Ok;

        case QueryStatus::PARSE_ERROR:
            return ParseError;

        case QueryStatus::QUERY_PLAN_ERROR:
            return PlanError;

        case QueryStatus::EXEC_ERROR:
            return ExecError;

        case QueryStatus::BUFFER_OVERFLOW:
            return BufferOverflow;

        default:
            return UnknownError;
    }
}

bool writeObjectBegin(Buffer::Writer writer) {
    return writer.writeChar('{');
}

bool writeObjectEnd(Buffer::Writer writer) {
    return writer.writeChar('}');
}

bool writeSymbol(const Symbol* sym, Buffer::Writer writer) {
    const std::string& symName = sym->getName();
    const size_t symSize = symName.size();
    if (writer.getBufferSize() < symSize+3) {
        return false;
    }

    char* buffer = writer.getBuffer();
    size_t bytesWritten = 0;

    {
        *buffer = '"';
        bytesWritten++;
        buffer++;
    }

    {
        strcpy(buffer, symName.c_str());
        bytesWritten += symSize;
        buffer += symSize;
    }

    {
        *buffer = '"';
        bytesWritten++;
        buffer++;
    }
    {
        *buffer = ':';
        bytesWritten++;
        buffer++;
    }
    
    writer.setWrittenBytes(bytesWritten);
    return true;
}

bool writeValue(const Value& value, Buffer::Writer writer) {
    switch (value.getType().getKind()) {
        case ValueType::VK_INT:
            return writer.writeString(std::to_string(value.getInt()));
        break;

        case ValueType::VK_UNSIGNED:
            return writer.writeString(std::to_string(value.getUint()));
        break;

        case ValueType::VK_BOOL:
        {
            if (value.getBool()) {
                return writer.writeString("true");
            } else {
                return writer.writeString("false");
            }
        }
        break;

        case ValueType::VK_DECIMAL:
            return writer.writeString(std::to_string(value.getDouble()));
        break;

        case ValueType::VK_STRING_REF:
        {
            const auto strRef = value.getStringRef();
            const auto size = strRef.size();
            const auto writtenSize = size+2;
            if (writer.getBufferSize() < writtenSize) {
                return false;
            }

            char* buffer = writer.getBuffer();
            *buffer = '\"';
            buffer++;
            strcpy(buffer, strRef.begin());
            buffer += size;
            *buffer = '"';
            writer.setWrittenBytes(writtenSize);
        }
        break;

        case ValueType::VK_STRING:
        {
            const std::string& str = value.getString();
            const auto size = str.size();
            const auto writtenSize = size+2;
            if (writer.getBufferSize() < writtenSize) {
                return false;
            }

            char* buffer = writer.getBuffer();
            *buffer = '\"';
            buffer++;
            strcpy(buffer, str.data());
            buffer += size;
            *buffer = '"';
            buffer++;
            writer.setWrittenBytes(writtenSize);
        }
        break;

        default:
            return writer.writeString("null");
        break;
    }

    return true;
}

}

Interpreter::Interpreter(InterpreterContext* interpCtxt)
    : _interpCtxt(interpCtxt)
{
}

Interpreter::~Interpreter() {
}

void Interpreter::execQuery(StringSpan query, Buffer* outBuffer) const {
    const auto timeExecStart = Clock::now();

    QueryParser parser;
    QueryCommand* cmd = parser.parse(query);
    if (!cmd) {
        return handleQueryError(QueryStatus::PARSE_ERROR, outBuffer);
    }

    Planner planner(_interpCtxt);
    PullPlan* pullPlan = planner.makePlan(cmd);
    if (!pullPlan) {
        return handleQueryError(QueryStatus::QUERY_PLAN_ERROR, outBuffer);
    }
    
    const auto timeExecEnd = Clock::now();
    const std::chrono::duration<double, std::milli> duration = timeExecEnd - timeExecStart;

    const bool writeRes = writeResponse(outBuffer, pullPlan, duration.count());
    if (!writeRes) {
        handleQueryError(QueryStatus::BUFFER_OVERFLOW, outBuffer);
    }

    // Cleanup
    delete pullPlan;

    return;
}

bool Interpreter::writeResponse(Buffer* outBuffer,
                                PullPlan* pullPlan,
                                double execTime) const {
    auto writer = outBuffer->getWriter();

    // Write http response header
    if (!writer.writeString(headerOk)) {
        return false;
    }
    if (!writer.writeString(emptyLine)) {
        return false;
    }

    // Begin
    if (!writer.writeString(bodyBegin)) {
        return false;
    }

    // Status
    const std::string& statusStr = getQueryStatusString(QueryStatus::OK);
    if (!writer.writeString(statusStr)) {
        return false;
    }

    // Time
    if (!writer.writeString(timeBeginStr)) {
        return false;
    }

    const std::string durationStr = std::to_string(execTime)+"ms";
    if (!writer.writeString(durationStr)) {
        return false;
    }

    if (!writer.writeString(timeEndStr)) {
        return false;
    }

    // Data begin
    if (!writer.writeString(bodyDataBegin)) {
        return false;
    }

    const SymbolTable* symTable = pullPlan->getSymbolTable();
    while (pullPlan->pull() != PullStatus::DONE) {
        const Frame& frame = pullPlan->getFrame();

        if (!writeObjectBegin(writer)) {
            return false;
        }

        for (const Symbol* symbol : symTable->symbols()) {
            const Value& value = frame[symbol];
            if (!writeSymbol(symbol, writer)) {
                return false;
            }
            if (!writeValue(value, writer)) {
                return false;
            }
        }

        if (!writeObjectEnd(writer)) {
            return false;
        }
    }

    // End
    if (!writer.writeString(bodyEnd)) {
        return false;
    }

    return true;
}

void Interpreter::handleQueryError(QueryStatus status, Buffer* outBuffer) const {
    auto writer = outBuffer->getWriter();
    writer.reset();

    // Write http response header
    strcpy(writer.getBuffer(), headerOk.c_str());
    writer.setWrittenBytes(headerOk.size()); 
    strcpy(writer.getBuffer(), emptyLine.c_str());
    writer.setWrittenBytes(emptyLine.size());

    // Begin
    strcpy(writer.getBuffer(), bodyBegin.c_str());
    writer.setWrittenBytes(bodyBegin.size());

    // Status
    const std::string_view statusStr = getQueryStatusString(status);
    strcpy(writer.getBuffer(), statusStr.data());
    writer.setWrittenBytes(statusStr.size());

    // Post status
    strcpy(writer.getBuffer(), bodyDataBegin.c_str());
    writer.setWrittenBytes(bodyDataBegin.size());

    // End
    strcpy(writer.getBuffer(), bodyEnd.c_str());
    writer.setWrittenBytes(bodyEnd.size());
}
