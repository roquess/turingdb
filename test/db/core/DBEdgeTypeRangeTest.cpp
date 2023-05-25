#include "DBEdgeTypeRange.h"
#include "DBAccessor.h"
#include "EdgeType.h"
#include "Writeback.h"

#include "gtest/gtest.h"

namespace db {

class DBEdgeTypeRangeTest : public ::testing::Test {
protected:
    void SetUp() override {
        _db = DB::create();
        Writeback wb(_db);

        const StringRef cellularLocName = _db->getString("CellularLocation");
        NodeType* cellularLoc =
            wb.createNodeType(cellularLocName);

        const StringRef metaboliteName = _db->getString("metabolite");
        NodeType* metabolite =
            wb.createNodeType(metaboliteName);

        for (size_t i = 0; i < 10; i++) {
            wb.createEdgeType(_db->getString(std::to_string(i) + "_EdgeType"),
                              cellularLoc, metabolite);
        }
    }

    void TearDown() override {
        delete _db;
    }

    db::DB* _db{nullptr};
};

TEST_F(DBEdgeTypeRangeTest, EdgeTypeIteration) {
    const DBAccessor accessor{_db};

    size_t i = 0;
    for (EdgeType* edgeType : accessor.edgeTypes()) {
        ASSERT_EQ(edgeType->getName().getSharedString()->getString(),
                  std::to_string(i) + "_EdgeType");
        i++;
    }
}

} // namespace db
