#include "gtest/gtest.h"

#include "LabelMap.h"

using namespace db;

class LabelMapTest : public ::testing::Test {
};

TEST_F(LabelMapTest, createOne) {
    LabelMap labelMap;

    const LabelID protein = labelMap.getLabel("protein");
    ASSERT_TRUE(protein.isValid());
}

TEST_F(LabelMapTest, identical) {
    LabelMap labelMap;

    const LabelID protein = labelMap.getLabel("protein");
    ASSERT_TRUE(protein.isValid());

    const LabelID protein2 = labelMap.getLabel("protein");
    ASSERT_TRUE(protein2.isValid());

    ASSERT_EQ(protein, protein2);
}

TEST_F(LabelMapTest, different) {
    LabelMap labelMap;

    const LabelID protein = labelMap.getLabel("protein");
    ASSERT_TRUE(protein.isValid());

    const LabelID protein2 = labelMap.getLabel("protein2");
    ASSERT_TRUE(protein2.isValid());

    ASSERT_NE(protein, protein2);
}

TEST_F(LabelMapTest, emptyString) {
    LabelMap map;

    const auto id = map.getLabel("");
    ASSERT_TRUE(id.isValid());
}
