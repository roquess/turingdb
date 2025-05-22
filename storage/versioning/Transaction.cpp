#include "Transaction.h"

#include "views/GraphView.h"
#include "reader/GraphReader.h"

using namespace db;

ReadTransaction::ReadTransaction() = default;

ReadTransaction::~ReadTransaction() = default;

GraphView ReadTransaction::viewGraph() const {
    return GraphView {*_data};
}

GraphReader ReadTransaction::readGraph() const {
    return GraphView {*_data}.read();
}

WriteTransaction::WriteTransaction() = default;

WriteTransaction::WriteTransaction(const WeakArc<const CommitData>& data,
                                   CommitBuilder* builder,
                                   DataPartBuilder* partBuilder,
                                   Change::Accessor&& changeAccessor)
    : _data(data),
    _builder(builder),
    _partBuilder(partBuilder),
    _changeAccessor(std::move(changeAccessor))
{
}

WriteTransaction::~WriteTransaction() = default;

GraphView WriteTransaction::viewGraph() const {
    return GraphView {*_data};
}

GraphReader WriteTransaction::readGraph() const {
    return GraphView {*_data}.read();
}
