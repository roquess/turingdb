#pragma once

#include "Range.h"

#include "Network.h"

namespace db {

class NetworkAccessor;

class NetworkEdgeRange {
public:
    friend NetworkAccessor;
    using BaseRange = STLRange<Network::Edges>;
    using Iterator = typename BaseRange::Iterator;

    NetworkEdgeRange() = default;

    bool empty() const { return _range.empty(); }
    size_t size() const { return _range.size(); }

    Iterator begin() const { return _range.begin(); }
    Iterator end() const { return _range.end(); }

private:
    STLRange<Network::Edges> _range;

    NetworkEdgeRange(const Network* net);
};

}
