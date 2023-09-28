#include "DBServerHandler.h"

#include <iostream>

#include "Interpreter.h"

using namespace db;

void DBServerHandler::process(Buffer* outBuffer,
                              const std::string& uri,
                              StringSpan payload) {
    _interp->execQuery(payload, outBuffer);
}
