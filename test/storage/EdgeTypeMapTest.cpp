#include "gtest/gtest.h"

#include "types/EdgeTypeMap.h"

using namespace db;

class EdgeTypeMapTest : public ::testing::Test {
};

TEST_F(EdgeTypeMapTest, createOne) {
    EdgeTypeMap edgeTypeMap;

    const EdgeTypeID knows = edgeTypeMap.getOrCreate("knows");
    ASSERT_TRUE(knows.isValid());
}

TEST_F(EdgeTypeMapTest, get) {
    EdgeTypeMap edgeTypeMap;

    edgeTypeMap.getOrCreate("knows");
    const auto knows = edgeTypeMap.get("knows");
    ASSERT_TRUE(knows);
}

TEST_F(EdgeTypeMapTest, different) {
    EdgeTypeMap edgeTypeMap;

    const EdgeTypeID knows = edgeTypeMap.getOrCreate("knows");
    const EdgeTypeID associated = edgeTypeMap.getOrCreate("associated");

    ASSERT_TRUE(knows.isValid());
    ASSERT_TRUE(associated.isValid());

    ASSERT_NE(knows, associated);
}

TEST_F(EdgeTypeMapTest, emptyString) {
    EdgeTypeMap map;

    const EdgeTypeID id = map.getOrCreate("");
    ASSERT_TRUE(id.isValid());
}
