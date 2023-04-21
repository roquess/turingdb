#include "NetworkNodeRange.h"

using namespace db;

NetworkNodeRange::NetworkNodeRange(const Network* net)
    : _range(&net->_nodes)
{
}
