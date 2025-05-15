#include "Transaction.h"

#include "views/GraphView.h"
#include "reader/GraphReader.h"

using namespace db;

Transaction::Transaction() = default;

Transaction::~Transaction() = default;

GraphView Transaction::viewGraph() const {
    return GraphView {*_data};
}

GraphReader Transaction::readGraph() const {
    return GraphView {*_data}.read();
}
