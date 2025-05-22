#include "GraphComparator.h"

#include "Graph.h"
#include "reader/GraphReader.h"
#include "DataPartComparator.h"
#include "GraphMetadataComparator.h"
#include "versioning/Transaction.h"

using namespace db;


bool GraphComparator::same(const Graph& a, const Graph& b) {
    if (a.getName() != b.getName()) {
        return false;
    }

    const FrozenCommitTx txA = a.openTransaction();
    const FrozenCommitTx txB = b.openTransaction();
    const GraphReader readerA = txA.readGraph();
    const GraphReader readerB = txB.readGraph();

    if (!GraphMetadataComparator::same(readerA.getMetadata(), readerB.getMetadata())) {
        return false;
    }

    const DataPartSpan partsA = readerA.dataparts();
    const DataPartSpan partsB = readerB.dataparts();

    if (partsA.size() != partsB.size()) {
        return false;
    }

    for (size_t i = 0; i < partsA.size(); i++) {
        if (!DataPartComparator::same(*partsA[i], *partsB[i])) {
            return false;
        }
    }

    return true;
}

