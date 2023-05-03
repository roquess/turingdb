#include "DBNetworkRange.h"

using namespace db;

DBNetworkRange::DBNetworkRange(const DB* db)
    : _range(&db->_networks)
{
}
