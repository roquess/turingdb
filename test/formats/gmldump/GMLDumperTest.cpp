// Copyright 2023 Turing Biosystems Ltd.

#include "gtest/gtest.h"

#include "DB.h"
#include "Network.h"
#include "Writeback.h"
#include "NodeType.h"
#include "NetworkAccessor.h"
#include "Node.h"

#include "GMLDumper.h"

using namespace db;

TEST(GMLDumper, create1) {
    DB* db = DB::create();

    Writeback wb(db);
    Network* net = wb.createNetwork(db->getString("my net"));
    ASSERT_TRUE(net);

    NodeType* nodeType = wb.createNodeType(db->getString("Node"));
    EdgeType* edgeType = wb.createEdgeType(db->getString("Edge"), nodeType, nodeType);

    Node* n1 = wb.createNode(net, nodeType);
    Node* n2 = wb.createNode(net, nodeType);
    Node* n3 = wb.createNode(net, nodeType);
    Node* n4 = wb.createNode(net, nodeType);
    Node* n5 = wb.createNode(net, nodeType);

    wb.createEdge(edgeType, n1, n2);
    wb.createEdge(edgeType, n1, n3);
    wb.createEdge(edgeType, n2, n4);
    wb.createEdge(edgeType, n3, n4);
    wb.createEdge(edgeType, n4, n5);

    NetworkAccessor netAcc(net);
    ASSERT_EQ(netAcc.nodes().size(), 5);
    ASSERT_FALSE(netAcc.nodes().empty());
    ASSERT_EQ(netAcc.edges().size(), 5);
    ASSERT_FALSE(netAcc.edges().empty());

    const auto gmlDumpPath = "net.gml";
    GMLDumper gmlDumper(net, gmlDumpPath);
    const bool dumpRes = gmlDumper.dump();
    EXPECT_TRUE(dumpRes);

    delete db;
}
