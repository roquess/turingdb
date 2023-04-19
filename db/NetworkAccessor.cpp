#include "NetworkAccessor.h"

using namespace db;

NetworkAccessor::NetworkAccessor(const Network* net)
    : _net(net)
{
}

NetworkAccessor::~NetworkAccessor() {
}
