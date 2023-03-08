// Copyright 2023 Turing Biosystems Ltd.

#include "gtest/gtest.h"

#include "DB.h"
#include "ValueType.h"

using namespace db;

TEST(ValueTypesTest, primitiveTypes) {
    DB* db = DB::create();
    ASSERT_TRUE(db);

    EXPECT_TRUE(db->getInt());
    EXPECT_TRUE(db->getInt()->isInt());
    EXPECT_EQ(db->getInt()->getKind(), ValueType::VK_INT);

    EXPECT_TRUE(db->getUnsigned());
    EXPECT_TRUE(db->getUnsigned()->isUnsigned());
    EXPECT_EQ(db->getUnsigned()->getKind(), ValueType::VK_UNSIGNED);

    EXPECT_TRUE(db->getBool());
    EXPECT_TRUE(db->getBool()->isBool());
    EXPECT_EQ(db->getBool()->getKind(), ValueType::VK_BOOL);

    EXPECT_TRUE(db->getDecimal());
    EXPECT_TRUE(db->getDecimal()->isDecimal());
    EXPECT_EQ(db->getDecimal()->getKind(), ValueType::VK_DECIMAL);

    EXPECT_TRUE(db->getString());
    EXPECT_TRUE(db->getString()->isString());
    EXPECT_EQ(db->getString()->getKind(), ValueType::VK_STRING);

    delete db;
}
