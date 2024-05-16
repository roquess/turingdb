#include "TestUtils.h"

#include "DBAccess.h"
#include "DataBuffer.h"

using namespace db;

void TestUtils::generateMillionTestDB(DBAccess& acc) {
    const size_t nodeCount = 1000000;

    DataBuffer buf = acc.newDataBuffer();
    const LabelsetID labelsetID = acc.getLabelsetID(Labelset::fromList({0}));

    for (size_t i = 0; i < nodeCount; i++) {
        buf.addNode(labelsetID);
    }

    acc.pushDataPart(buf);
}
