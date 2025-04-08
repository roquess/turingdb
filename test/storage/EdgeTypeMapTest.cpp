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

TEST_F(EdgeTypeMapTest, swap) {
    EdgeTypeMap edgeTypeMap;

    edgeTypeMap.getOrCreate("Knows");
    edgeTypeMap.getOrCreate("Associated");

    ASSERT_EQ(edgeTypeMap.get("Knows").value(), 0);
    ASSERT_EQ(edgeTypeMap.get("Associated").value(), 1);

    ASSERT_TRUE(edgeTypeMap.setID("Knows", 1));
    ASSERT_FALSE(edgeTypeMap.setID("Knows", 2));

    ASSERT_EQ(edgeTypeMap.get("Knows").value(), 1);
    ASSERT_EQ(edgeTypeMap.get("Associated").value(), 0);

    const auto AssociatedName = edgeTypeMap.getName(0);
    ASSERT_TRUE(AssociatedName);
    fmt::print("\"{}\" ?= \"Associated\"\n", *AssociatedName);
    ASSERT_TRUE(*AssociatedName == "Associated");

    const auto KnowsName = edgeTypeMap.getName(1);
    ASSERT_TRUE(KnowsName);
    fmt::print("\"{}\" ?= \"Knows\"\n", *KnowsName);
    ASSERT_TRUE(*KnowsName == "Knows");
}
