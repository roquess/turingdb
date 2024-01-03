#include "EdgeMap.h"

using namespace db;

EdgeMap::EdgeMap()
{
}

EdgeMap::~EdgeMap() {
}

void EdgeMap::insert(LabelID label, Edge* edge) {
    _map[label].emplace_back(edge);
}