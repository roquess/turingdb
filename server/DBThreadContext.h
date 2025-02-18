#pragma once

#include "AbstractThreadContext.h"
#include "LocalMemory.h"

namespace db {

class DBThreadContext : public net::AbstractThreadContext {
public:
    DBThreadContext() = default;

    ~DBThreadContext() override = default;

    DBThreadContext(const DBThreadContext&) = delete;
    DBThreadContext(DBThreadContext&&) = delete;
    DBThreadContext& operator=(const DBThreadContext&) = delete;
    DBThreadContext& operator=(DBThreadContext&&) = delete;

    LocalMemory& getLocalMemory() { return _localMem; }

private:
    LocalMemory _localMem;
};

}
