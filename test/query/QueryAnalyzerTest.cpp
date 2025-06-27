#include <gtest/gtest.h>

#include "ID.h"
#include "LocalMemory.h"
#include "QueryInterpreter.h"
#include "QueryTester.h"
#include "TuringDB.h"
#include "SimpleGraph.h"
#include "TuringTest.h"

using namespace db;

class QueryAnalyzerTest : public turing::test::TuringTest {
public:
    void initialize() override {
        SystemManager& sysMan = _db.getSystemManager();
        Graph* graph = sysMan.createGraph("simple");
        SimpleGraph::createSimpleGraph(graph);
        _interp = std::make_unique<QueryInterpreter>(&_db.getSystemManager(),
                                                     &_db.getJobSystem());
    }
    
protected:
    TuringDB _db;
    LocalMemory _mem;
    std::unique_ptr<QueryInterpreter> _interp {nullptr};
};

TEST_F(QueryAnalyzerTest, typeCheckInt64) {
    QueryTester tester {_mem, *_interp};

    // Query Int64 with Int64
    tester.query("MATCH n-[e{duration:20}]-m return e")
        .expectVector<EdgeID>({0, 1, 3, 4, 10})
        .execute();

    // Query Int64 with UInt64
    // NOTE: 26/6/25 @Remy @Cyrus decided not to support this coercion
    tester.query("MATCH n-[e{duration:20u}]-m return e")
        .expectError()
        .execute();

    // Query Int64 with String
    tester.query("MATCH n-[e{duration:\"20\"}]-m return e")
        .expectError()
        .execute();

    // Query Int64 with Bool
    tester.query("MATCH n-[e{duration:true}]-m return e")
        .expectError()
        .execute();

    // Query Int64 with Double
    tester.query("MATCH n-[e{duration:20.}]-m return e")
        .expectError()
        .execute();
}

// TODO: SimpleGraph has no UInt64 property types
/*
TEST_F(QueryAnalyzerTest, typeCheckUInt64) {
    QueryTester tester {_mem, *_interp};

    // Query UInt64 with UInt64
    tester.query("MATCH n-[e{duration:20}]-m return e")
        .expectVector<EdgeID>({0, 1, 3, 4, 10})
        .execute();

    // Query UInt64 with Int64
    tester.query("MATCH n-[e{duration:20u}]-m return e")
        .expectVector<EdgeID>({0, 1, 3, 4, 10})
        .execute();

    // Query UInt64 with String
    tester.query("MATCH n-[e{duration:\"20\"}]-m return e")
        .expectError()
        .execute();

    // Query UInt64 with Bool
    tester.query("MATCH n-[e{duration:true}]-m return e")
        .expectError()
        .execute();

    // Query UInt64 with Double
    tester.query("MATCH n-[e{duration:20.}]-m return e")
        .expectError()
        .execute();
}
*/

TEST_F(QueryAnalyzerTest, typeCheckString) {
    QueryTester tester {_mem, *_interp};

    // Query String with String
    tester.query("MATCH n:Person{name:\"Remy\"} return n")
        .expectVector<NodeID>({0})
        .execute();

    // Query String with Int64
    tester.query("MATCH n:Person{name:12} return n")
        .expectError()
        .execute();

    // Query String with UInt64
    tester.query("MATCH n:Person{name:12u} return n")
        .expectError()
        .execute();

    // Query String with Bool
    tester.query("MATCH n:Person{name:true} return n")
        .expectError()
        .execute();
    
    // Query String with Double
    tester.query("MATCH n:Person{name:20.} return n")
        .expectError()
        .execute();
}

TEST_F(QueryAnalyzerTest, typeCheckBool) {
    QueryTester tester {_mem, *_interp};

    // Query Bool with Bool
    tester.query("MATCH n:Interest{isReal:true} return n")
        .expectVector<NodeID>({2, 6, 10})
        .execute();

    // Query Bool with Int64
    tester.query("MATCH n:Interest{isReal:12} return n")
        .expectError()
        .execute();

    // Query Bool with UInt64
    tester.query("MATCH n:Interest{isReal:12u} return n")
        .expectError()
        .execute();

    // Query Bool with String
    tester.query("MATCH n:Interest{isReal:\"true\"} return n")
        .expectError()
        .execute();
    
    // Query Bool with Double
    tester.query("MATCH n:Interest{isReal:12.} return n")
        .expectError()
        .execute();
}

// TODO: SimpleGraph has no UInt64 property types
//TEST_F(QueryAnalyzerTest, typeCheckDouble){}
