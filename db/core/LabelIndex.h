#pragma once

#include <map>
#include <vector>

#include "LabelID.h"

namespace db {

class Node;
class Edge;

class LabelIndex {
public:
    LabelIndex();
    ~LabelIndex();

    void updateOnAddLabel(LabelID label, Node* node);
    void addEdge(LabelID label, Edge* edge);

private:
    std::map<LabelID, std::vector<Node*>> _nodeLabelMap;
    std::map<LabelID, std::vector<Edge*>> _edgeLabelMap;
};

}