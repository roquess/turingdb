#pragma once

#include "NetworkNodeRange.h"
#include "NetworkEdgeRange.h"

namespace db {

class Network;

class NetworkAccessor {
public:
    NetworkAccessor(const Network* net);
    ~NetworkAccessor();

    NetworkNodeRange nodes() const { return NetworkNodeRange(_net); }

    NetworkEdgeRange edges() const { return NetworkEdgeRange(_net); }

private:
    const Network* _net {nullptr};
};

}
