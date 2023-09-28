#pragma once

#include <string>

#include "StringSpan.h"

class Buffer;

namespace db {

class Interpreter;

class DBServerHandler {
public:
    DBServerHandler(Interpreter* interp)
        : _interp(interp)
    {
    }

    void process(Buffer* outBuffer,
                 const std::string& uri,
                 StringSpan payload);

private:
    Interpreter* _interp {nullptr};
};

}
