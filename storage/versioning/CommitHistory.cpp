#include "CommitHistory.h"

#include "CommitView.h"

using namespace db;

CommitHistory::CommitHistory() = default;

CommitHistory::~CommitHistory() = default;

std::span<const CommitView> CommitHistory::commits() const
{
    return std::span<const CommitView>(_commits);
}

