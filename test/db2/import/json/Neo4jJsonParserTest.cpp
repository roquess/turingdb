#include <gtest/gtest.h>
#include <regex>

#include "BioLog.h"
#include "DB.h"
#include "FileUtils.h"
#include "JsonParser.h"
#include "PerfStat.h"

using namespace db;
using namespace std::chrono;

#define MEASURE_TIME(msg, op)                       \
    t0 = std::chrono::high_resolution_clock::now(); \
    op;                                  \
    t1 = std::chrono::high_resolution_clock::now(); \
    printDuration<milliseconds>(msg, t0, t1);

class Neo4jJsonParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        const testing::TestInfo* const testInfo =
            testing::UnitTest::GetInstance()->current_test_info();

        _outDir = testInfo->test_suite_name();
        _outDir += "_";
        _outDir += testInfo->name();
        _outDir += ".out";
        _logPath = FileUtils::Path(_outDir) / "log";

        if (FileUtils::exists(_outDir)) {
            FileUtils::removeDirectory(_outDir);
        }
        FileUtils::createDirectory(_outDir);

        Log::BioLog::init();
        Log::BioLog::openFile(_logPath.string());

        _db = std::make_unique<DB>();
    }

    void TearDown() override {
        Log::BioLog::destroy();
    }

    std::unique_ptr<DB> _db = nullptr;
    std::string _outDir;
    FileUtils::Path _logPath;
};

template <typename T>
void printDuration(std::string_view msg, const auto& t0, const auto& t1) {
    if constexpr (std::is_same_v<T, microseconds>) {
        float dur = duration_cast<nanoseconds>(t1 - t0).count();
        std::cout << msg << ": " << dur / 1'000.0f << " us" << std::endl;
    } else if constexpr (std::is_same_v<T, milliseconds>) {
        float dur = duration_cast<nanoseconds>(t1 - t0).count();
        std::cout << msg << ": " << dur / 1'000'000.0f << " ms" << std::endl;
    } else if constexpr (std::is_same_v<T, seconds>) {
        float dur = duration_cast<nanoseconds>(t1 - t0).count();
        std::cout << msg << ": " << dur / 1'000'000'000.0f << " us" << std::endl;
    }
}

TEST_F(Neo4jJsonParserTest, General) {
    auto t0 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();
    static constexpr size_t nodeCountLimit = 1000000;
    static constexpr size_t edgeCountLimit = 3000000;

    JsonParser parser(_db.get());
    const std::string turingHome = std::getenv("TURING_HOME");
    const FileUtils::Path jsonDir = FileUtils::Path {turingHome} / "neo4j" / "pole-db";
    // const FileUtils::Path jsonDir = "/home/luclabarriere/reactome.out/json";

    auto statsPath = jsonDir / "stats.json";
    const auto [nodeCount, edgeCount] = JsonParser::parseStats(statsPath);
    // ASSERT_EQ(61521, nodeCount);
    // ASSERT_EQ(105840, edgeCount);

    auto nodePropertiesPath = jsonDir / "nodeProperties.json";
    ASSERT_TRUE(parser.parseNodeProperties(nodePropertiesPath));
    // ASSERT_EQ(27, _db->propTypeMap().getCount());

    auto edgePropertiesPath = jsonDir / "edgeProperties.json";
    ASSERT_TRUE(parser.parseEdgeProperties(edgePropertiesPath));
    //ASSERT_EQ(18, _db->edgeTypeMap().getCount());

    // Nodes
    std::vector<FileUtils::Path> nodeFiles;
    FileUtils::listFiles(jsonDir, nodeFiles);
    std::regex nodeRegex {"nodes_([0-9]*).json"};
    std::map<size_t, FileUtils::Path> nodeFilesOrdered;

    for (const FileUtils::Path& path : nodeFiles) {
        if (std::regex_search(path.string(), nodeRegex)) {
            size_t index = std::stoul(std::regex_replace(
                path.filename().string(),
                nodeRegex, "$1"));
            nodeFilesOrdered[index] = path;
        }
    }

    size_t nodeCountLeft = nodeCount;
    for (const auto& [index, path] : nodeFilesOrdered) {
        const size_t whole = (size_t)((nodeCountLeft / nodeCountLimit) != 0);
        const size_t modulo = nodeCountLeft % nodeCountLimit;
        const size_t fileNodeCount = whole * nodeCountLimit + !whole * modulo;
        nodeCountLeft -= fileNodeCount;

        MEASURE_TIME("Nodes " + std::to_string(index), parser.parseNodes(path, fileNodeCount););
    }

    // Edges
    std::vector<FileUtils::Path> edgeFiles;
    FileUtils::listFiles(jsonDir, edgeFiles);
    std::regex edgeRegex {"edges_([0-9]*).json"};
    std::map<size_t, FileUtils::Path> edgeFilesOrdered;

    for (const FileUtils::Path& path : edgeFiles) {
        if (std::regex_search(path.string(), edgeRegex)) {
            size_t index = std::stoul(std::regex_replace(
                path.filename().string(),
                edgeRegex, "$1"));
            edgeFilesOrdered[index] = path;
        }
    }

    size_t edgeCountLeft = edgeCount;
    for (const auto& [index, path] : edgeFilesOrdered) {
        const size_t whole = (size_t)((edgeCountLeft / edgeCountLimit) != 0);
        const size_t modulo = edgeCountLeft % edgeCountLimit;
        const size_t fileEdgeCount = whole * edgeCountLimit + !whole * modulo;
        edgeCountLeft -= fileEdgeCount;

        MEASURE_TIME("Edges " + std::to_string(index), parser.parseEdges(path, fileEdgeCount););
        break;
    }

    MEASURE_TIME("Build", auto buf = parser.build());
    MEASURE_TIME("Push", _db->uniqueAccess().pushDataPart(*buf));
}
