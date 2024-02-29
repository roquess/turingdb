#include "BioLog.h"
#include "FileUtils.h"
#include "LabelSet.h"

#include <gtest/gtest.h>

using namespace db;

TEST(LabelSetTest, LabelSet) {
    LabelSet set;
    set.set(0);
    set.set(5);
    set.set(50);
    ASSERT_TRUE(set.hasLabel(0));
    ASSERT_TRUE(set.hasLabel(5));
    ASSERT_TRUE(set.hasLabel(50));
    ASSERT_FALSE(set.hasLabel(4));
    ASSERT_FALSE(set.hasLabel(51));
    ASSERT_TRUE(set.hasAtLeastLabels({5}));
    ASSERT_TRUE(set.hasAtLeastLabels({0, 5}));
    ASSERT_TRUE(set.hasAtLeastLabels({0, 50}));
    ASSERT_TRUE(set.hasAtLeastLabels({0, 5, 50}));
    ASSERT_FALSE(set.hasAtLeastLabels({0, 1}));
}
