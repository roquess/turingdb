#include "NetworkEdgeRange.h"

using namespace db;

NetworkEdgeRange::NetworkEdgeRange(const Network* net)
    : _range(&net->_edges)
{
}
