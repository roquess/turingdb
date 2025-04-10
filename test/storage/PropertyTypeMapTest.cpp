#include "gtest/gtest.h"

#include "types/PropertyTypeMap.h"

using namespace db;

class PropertyTypeMapTest : public ::testing::Test {
};

TEST_F(PropertyTypeMapTest, createOne) {
    PropertyTypeMap propertyTypeMap;

    const PropertyType name = propertyTypeMap.getOrCreate("name", ValueType::String);
    ASSERT_TRUE(name.isValid());
}

TEST_F(PropertyTypeMapTest, get) {
    PropertyTypeMap propertyTypeMap;

    propertyTypeMap.getOrCreate("name", ValueType::String);
    const auto name = propertyTypeMap.get("name");
    ASSERT_TRUE(name);
}

TEST_F(PropertyTypeMapTest, different) {
    PropertyTypeMap propertyTypeMap;

    const PropertyType name = propertyTypeMap.getOrCreate("name", ValueType::String);
    const PropertyType age = propertyTypeMap.getOrCreate("age", ValueType::UInt64);

    ASSERT_TRUE(name.isValid());
    ASSERT_TRUE(age.isValid());

    ASSERT_NE(name._id, age._id);
}

TEST_F(PropertyTypeMapTest, emptyString) {
    PropertyTypeMap map;

    const PropertyType pt = map.getOrCreate("", ValueType::String);
    ASSERT_TRUE(pt.isValid());
}
