#pragma once

#include "Range.h"

#include "Network.h"

namespace db {

class NetworkAccessor;

class NetworkNodeRange {
public:
    friend NetworkAccessor;
    using BaseRange = STLRange<Network::Nodes>;
    using Iterator = typename BaseRange::Iterator;

    NetworkNodeRange() = default;

    bool empty() const { return _range.empty(); }
    size_t size() const { return _range.size(); }

    Iterator begin() const { return _range.begin(); }
    Iterator end() const { return _range.end(); }

private:
    STLRange<Network::Nodes> _range;

    NetworkNodeRange(const Network* net);
};

}
