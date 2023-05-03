#include "DBNetworkRange.h"

#include "DB.h"

using namespace db;

DBNetworkRange::DBNetworkRange(const DB* db)
    : _range(&db->_networks)
{
}
