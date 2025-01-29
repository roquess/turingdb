#include "gtest/gtest.h"

#include "labels/LabelMap.h"

using namespace db;

class LabelMapTest : public ::testing::Test {
};

TEST_F(LabelMapTest, createOne) {
    LabelMap labelMap;

    const LabelID protein = labelMap.create("protein");
    ASSERT_TRUE(protein.isValid());
}

TEST_F(LabelMapTest, get) {
    LabelMap labelMap;

    labelMap.create("protein");
    const LabelID protein = labelMap.get("protein");
    ASSERT_TRUE(protein.isValid());
}

TEST_F(LabelMapTest, different) {
    LabelMap labelMap;

    const LabelID protein1 = labelMap.create("protein1");
    const LabelID protein2 = labelMap.create("protein2");

    ASSERT_TRUE(protein1.isValid());
    ASSERT_TRUE(protein2.isValid());

    ASSERT_NE(protein1, protein2);
}

TEST_F(LabelMapTest, emptyString) {
    LabelMap map;

    const LabelID id = map.create("");
    ASSERT_TRUE(id.isValid());
}
