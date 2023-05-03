#pragma once

#include "DBNetworkRange.h"

namespace db {

class DB;

class DBAccessor {
public:
    DBAccessor(const DB* db);
    ~DBAccessor();

    DBNetworkRange networks() const { return DBNetworkRange(_db); }

private:
    const DB* _db {nullptr};
};

}
