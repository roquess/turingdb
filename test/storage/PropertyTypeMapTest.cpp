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

TEST_F(PropertyTypeMapTest, swap) {
    PropertyTypeMap propertyTypeMap;

    propertyTypeMap.getOrCreate("Name", ValueType::String);
    propertyTypeMap.getOrCreate("Age", ValueType::UInt64);

    ASSERT_EQ(propertyTypeMap.get("Name")->_id, 0);
    ASSERT_EQ(propertyTypeMap.get("Age")->_id, 1);

    ASSERT_TRUE(propertyTypeMap.setID("Name", 1));
    ASSERT_FALSE(propertyTypeMap.setID("Name", 2));

    ASSERT_EQ(propertyTypeMap.get("Name")->_id, 1);
    ASSERT_EQ(propertyTypeMap.get("Age")->_id, 0);

    const auto AgeName = propertyTypeMap.getName(0);
    ASSERT_TRUE(AgeName);
    fmt::print("\"{}\" ?= \"Age\"\n", *AgeName);
    ASSERT_TRUE(*AgeName == "Age");

    const auto NameName = propertyTypeMap.getName(1);
    ASSERT_TRUE(NameName);
    fmt::print("\"{}\" ?= \"Name\"\n", *NameName);
    ASSERT_TRUE(*NameName == "Name");
}
