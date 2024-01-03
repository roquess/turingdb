#pragma once

#include <map>
#include <vector>

#include "LabelID.h"

namespace db {

class Edge;

class EdgeMap {
public:
    EdgeMap();
    ~EdgeMap();

    bool empty() const { return _map.empty(); }

    auto begin() const { return _map.cbegin(); }
    auto end() const { return _map.cend(); }

    auto find(LabelID label) const { return _map.find(label); }

    void insert(LabelID label, Edge* edge);

private:
    std::map<LabelID, std::vector<Edge*>> _map;
};

}