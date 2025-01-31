#pragma once

#include <string_view>

#include "QueryStatus.h"
#include "QueryCallback.h"

namespace db {

class SystemManager;
class LocalMemory;

class QueryInterpreter {
public:
    QueryInterpreter(SystemManager* sysMan);
    ~QueryInterpreter();

    QueryStatus execute(std::string_view query,
                        std::string_view graphName,
                        LocalMemory* mem,
                        QueryCallback callback);

private:
    SystemManager* _sysMan {nullptr};
};

}
