#pragma once

#include "ArcManager.h"
#include "CommitData.h"

namespace db {

class GraphView;
class GraphReader;
class VersionController;
class CommitBuilder;

class Transaction {
public:
    Transaction();
    ~Transaction();

    Transaction(const WeakArc<const CommitData>& data)
        : _data(data)
    {
    }

    Transaction(const Transaction&) = default;
    Transaction(Transaction&&) = default;
    Transaction& operator=(const Transaction&) = default;
    Transaction& operator=(Transaction&&) = default;

    [[nodiscard]] bool isValid() const { return _data != nullptr; }

    [[nodiscard]] GraphView viewGraph() const;
    [[nodiscard]] GraphReader readGraph() const;
    [[nodiscard]] const CommitData& commitData() const { return *_data; }

private:
    WeakArc<const CommitData> _data;
};

}

