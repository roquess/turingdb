#pragma once

#include <string>
#include <string_view>

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
                 std::string_view payload);

private:
    Interpreter* _interp {nullptr};
};

}
