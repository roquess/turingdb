#include "LabelIndex.h"

using namespace db;

LabelIndex::LabelIndex()
{
}

LabelIndex::~LabelIndex() {
}

void LabelIndex::updateOnAddLabel(LabelID label, Node* node) {
    const auto it = _nodeLabelMap.find(label);
    if (it == _nodeLabelMap.end()) {
        return;
    }

    it->second.emplace_back(node);
}

void LabelIndex::addEdge(LabelID label, Edge* edge) {
    const auto it = _edgeLabelMap.find(label);
    if (it == _edgeLabelMap.end()) {
        return;
    }

    it->second.emplace_back(edge);
}