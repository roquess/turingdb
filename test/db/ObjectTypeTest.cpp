// Copyright 2023 Turing Biosystems Ltd.

#include "gtest/gtest.h"

#include "DB.h"
#include "ObjectType.h"
#include "LinkType.h"

using namespace db;

TEST(ObjectTypeTest, createEmpty) {
    DB* db = DB::create();

    const StringRef anyName = db->getString("any");
    ObjectType* objType = ObjectType::create(db, anyName);
    EXPECT_TRUE(objType);
    EXPECT_EQ(objType->getName(), anyName);
    EXPECT_TRUE(objType->getPropertyTypes().empty());
    EXPECT_TRUE(objType->getInLinkTypes().empty());
    EXPECT_TRUE(objType->getOutLinkTypes().empty());

    delete db;
}

TEST(ObjectTypeTest, createSame) {
    DB* db = DB::create();

    const StringRef anyName = db->getString("any");
    ObjectType* objType = ObjectType::create(db, anyName); 
    EXPECT_TRUE(objType);

    ObjectType* objType2 = ObjectType::create(db, anyName); 
    EXPECT_FALSE(objType2);

    delete db;
}

TEST(ObjectTypeTest, links) {
    DB* db = DB::create();

    const StringRef obj1 = db->getString("obj1");
    ObjectType* objType1 = ObjectType::create(db, obj1);
    ASSERT_TRUE(objType1);

    const StringRef obj2 = db->getString("obj2");
    ObjectType* objType2 = ObjectType::create(db, obj2);
    ASSERT_TRUE(objType2);

    const StringRef isAssociatedWith = db->getString("isAssociatedWith");
    LinkType* linkType1 = LinkType::create(db, isAssociatedWith, objType1, objType2);
    ASSERT_TRUE(linkType1);
    EXPECT_EQ(linkType1->getSourceType(), objType1);
    EXPECT_EQ(linkType1->getTargetType(), objType2);

    EXPECT_TRUE(objType1->getInLinkTypes().empty());
    EXPECT_EQ(objType1->getOutLinkTypes(), std::vector<LinkType*>{linkType1});
    EXPECT_TRUE(objType2->getOutLinkTypes().empty());
    EXPECT_EQ(objType2->getInLinkTypes(), std::vector<LinkType*>{linkType1});

    const StringRef isRegulatedBy = db->getString("isRegulatedBy");
    LinkType* linkType2 = LinkType::create(db, isRegulatedBy, objType2, objType1);
    ASSERT_TRUE(linkType2);
    EXPECT_EQ(objType2->getOutLinkTypes(), std::vector<LinkType*>{linkType2});
    EXPECT_EQ(objType1->getInLinkTypes(), std::vector<LinkType*>{linkType2});

    delete db;
}
