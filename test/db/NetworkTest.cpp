// Copyright 2023 Turing Biosystems Ltd.

#include "gtest/gtest.h"

#include "DB.h"
#include "Network.h"
#include "ObjectType.h"
#include "Object.h"
#include "LinkType.h"
#include "Link.h"

using namespace db;

TEST(NetworkTest, createEmpty) {
    DB* db = DB::create();

    Network* net = Network::create(db, db->getString("my net"));
    ASSERT_TRUE(net);
    EXPECT_EQ(net->getDisplayName(), db->getString("my net"));
    EXPECT_TRUE(net->objects().empty());
    EXPECT_TRUE(net->internalLinks().empty());
    EXPECT_TRUE(net->inLinks().empty());
    EXPECT_TRUE(net->outLinks().empty());
    EXPECT_TRUE(net->subNetworks().empty());
    EXPECT_FALSE(net->getParent());

    delete db;
}

TEST(NetworkTest, ppiCXCL14) {
    DB* db = DB::create();

    // Protein
    ObjectType* protein = ObjectType::create(db, db->getString("Protein"));
    LinkType* interactWith = LinkType::create(db,
                                              db->getString("interactWith"),
                                              protein,
                                              protein);

    Network* ppi = Network::create(db, db->getString("ppi"));

    Object* CXCL14 = Object::create(ppi, protein, db->getString("CXCL14"));
    Object* CXCL12 = Object::create(ppi, protein, db->getString("CXCL12"));
    Object* CCR7 = Object::create(ppi, protein, db->getString("CCR7"));
    Object* CXCR2 = Object::create(ppi, protein, db->getString("CXCR2"));
    Object* CXCR1 = Object::create(ppi, protein, db->getString("CXCR1"));
    Object* CXCR5 = Object::create(ppi, protein, db->getString("CXCR5"));
    Object* CCR2 = Object::create(ppi, protein, db->getString("CCR2"));
    Object* CCR1 = Object::create(ppi, protein, db->getString("CCR1"));
    Object* CCR5 = Object::create(ppi, protein, db->getString("CCR5"));
    Object* CXCR4 = Object::create(ppi, protein, db->getString("CXCR4"));
    Object* CXCR3 = Object::create(ppi, protein, db->getString("CXCR3"));

    Link::create(db, interactWith, CXCL14, CXCL12);
    Link::create(db, interactWith, CXCR3, CXCL14);
    Link::create(db, interactWith, CXCL12, CXCR5);
    Link::create(db, interactWith, CXCL12, CXCR1);
    Link::create(db, interactWith, CXCL12, CXCR2);
    Link::create(db, interactWith, CXCR1, CXCR2);
    Link::create(db, interactWith, CXCL14, CXCR1);
    Link::create(db, interactWith, CXCL14, CXCR2);
    Link::create(db, interactWith, CCR7, CXCL14);
    Link::create(db, interactWith, CCR7, CXCL12);
    Link::create(db, interactWith, CXCR3, CXCR4);
    Link::create(db, interactWith, CXCR4, CCR5);
    Link::create(db, interactWith, CXCL14, CXCR4);
    Link::create(db, interactWith, CXCL12, CXCR4);
    Link::create(db, interactWith, CCR5, CCR1);
    Link::create(db, interactWith, CCR5, CCR2);
    Link::create(db, interactWith, CCR1, CCR2);
    Link::create(db, interactWith, CXCL14, CCR5);
    Link::create(db, interactWith, CXCL12, CCR5);
    Link::create(db, interactWith, CXCL14, CCR1);
    Link::create(db, interactWith, CXCL12, CCR1);
    Link::create(db, interactWith, CXCL14, CCR2);
    Link::create(db, interactWith, CXCL12, CCR2);

    delete db;
}
