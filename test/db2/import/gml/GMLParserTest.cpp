#include "TuringTest.h"
#include "GMLParser.h"
#include "DB.h"
#include "JobSystem.h"

using namespace db;

class GMLParserTest : public TuringTest {
protected:
    void initialize() override {
        _jobSystem = std::make_unique<JobSystem>();
        _jobSystem->initialize();
    }

    void terminate() override {
        _jobSystem->terminate();
    }

    std::unique_ptr<JobSystem> _jobSystem;
};

GMLParser::Result importDB(std::string_view data) {
    auto db = std::make_unique<DB>();
    GMLParser parser(db.get());
    return parser.parse(data);
}

void expectError(const GMLParser::Result& res) {
    ASSERT_FALSE(res);
    const auto& err = res.error();
    spdlog::info("[Expected error] {}", err.getMessage());
}

TEST_F(GMLParserTest, Empty) {
    ASSERT_TRUE(importDB("graph []"));
    ASSERT_TRUE(importDB(" graph []"));
    ASSERT_TRUE(importDB(" graph [] "));
    ASSERT_TRUE(importDB("\tgraph [] "));
    ASSERT_TRUE(importDB("\tgraph [] \n"));
    ASSERT_TRUE(importDB("\ngraph [] \n"));
    ASSERT_TRUE(importDB("\n\n\r graph [] \n"));
    ASSERT_FALSE(importDB("\n\n\r graph [] dsqd\n"));

    expectError(importDB("\tdsqd [] \n"));

    {
        auto res = importDB("\n\ndsqd [] \n");
        expectError(res);
        ASSERT_EQ(res.error().getLine(), 2);
    }
}

TEST_F(GMLParserTest, Nodes) {
    {
        expectError(importDB(
            "graph [\n"
            "  node [\n"
            "]"));
        ASSERT_TRUE(importDB("graph [\n"
                             "  node []\n"
                             "]"));
    }
}
