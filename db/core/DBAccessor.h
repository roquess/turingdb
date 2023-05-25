#pragma once

#include "DBNetworkRange.h"
#include "DBNodeTypeRange.h"
#include "DBEdgeTypeRange.h"

namespace db {

class DB;

class DBAccessor {
public:
    DBAccessor(const DB* db);
    ~DBAccessor();

    DBNetworkRange networks() const { return DBNetworkRange(_db); }
    DBNodeTypeRange nodeTypes() const { return DBNodeTypeRange(_db); }
    DBEdgeTypeRange edgeTypes() const { return DBEdgeTypeRange(_db); }

private:
    const DB* _db {nullptr};
};

}
