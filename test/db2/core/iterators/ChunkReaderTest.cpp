#include "BioLog.h"
#include "DB.h"
#include "FileUtils.h"
#include "PerfStat.h"
#include "Reader.h"
#include "TemporaryDataBuffer.h"
#include "TimerStat.h"
#include "iterators/GetEdgesChunkReader.h"
#include "iterators/GetInEdgesChunkReader.h"
#include "iterators/GetOutEdgesChunkReader.h"
#include "iterators/ScanEdgesChunkReader.h"
#include "iterators/ScanNodesChunkReader.h"

#include <gtest/gtest.h>

using namespace db;
using namespace std;
using namespace chrono;

class ChunkReaderTest : public ::testing::Test {
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

        _db = new DB;

        TemporaryDataBuffer tempData1(0, 0);
        _nodeIDs.insert(tempData1.addNode());        // Node 0
        _nodeIDs.insert(tempData1.addNode());        // Node 1
        _nodeIDs.insert(tempData1.addNode());        // Node 2
        _edgeIDs.insert(tempData1.addEdge(0, 0, 1)); // Edge 0
        _edgeIDs.insert(tempData1.addEdge(0, 0, 2)); // Edge 1

        // Concurrent writing
        TemporaryDataBuffer tempData2(0, 0);
        _nodeIDs.insert(tempData2.addNode());        // Node 3
        _nodeIDs.insert(tempData2.addNode());        // Node 4
        _edgeIDs.insert(tempData2.addEdge(0, 0, 1)); // Edge 2 [3->4]
        _edgeIDs.insert(tempData2.addEdge(0, 0, 1)); // Edge 3 [3->4]
        _edgeIDs.insert(tempData2.addEdge(0, 1, 0)); // Edge 4 [4->3]

        TemporaryDataBuffer tempData3(5, 5); // Empty buffer
        tempData3.setNextNodeID(5);
        tempData3.setNextEdgeID(5);

        TemporaryDataBuffer tempData4(5, 5);
        tempData4.setNextNodeID(5);
        tempData4.setNextEdgeID(5);
        _nodeIDs.insert(tempData4.addNode());        // Node 5
        _nodeIDs.insert(tempData4.addNode());        // Node 6
        _nodeIDs.insert(tempData4.addNode());        // Node 7
        _nodeIDs.insert(tempData4.addNode());        // Node 8
        _edgeIDs.insert(tempData4.addEdge(0, 6, 3)); // Edge 5
        _edgeIDs.insert(tempData4.addEdge(0, 6, 8)); // Edge 6
        _edgeIDs.insert(tempData4.addEdge(0, 7, 8)); // Edge 7
        // Reference node in previous datapart
        _edgeIDs.insert(tempData4.addEdge(0, 2, 8)); // Edge 8

        _db->pushDataPart(tempData1);
        _db->pushDataPart(tempData2);
        _db->pushDataPart(tempData3);
        _db->pushDataPart(tempData4);
    }

    void TearDown() override {
        delete _db;
        Log::BioLog::destroy();
    }

    std::set<EntityID> _nodeIDs;
    std::set<EntityID> _edgeIDs;
    DB* _db = nullptr;
    std::string _outDir;
    FileUtils::Path _logPath;
};

TEST_F(ChunkReaderTest, ScanEdgesChunkReaderTest) {
    auto access = _db->access();
    auto reader = access.getReader();
    ColumnEdges column;
    auto chunkReader = reader.scanEdges(&column);
    chunkReader.prepare();

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        while (chunkReader.getStatus() != ChunkReader::Status::Finished) {
            chunkReader.work();
        }

        std::string output;
        for (const auto& edge : column) {
            output += std::to_string(edge.edgeID.getID());
            output += std::to_string(edge.nodeID.getID());
            output += std::to_string(edge.otherID.getID());
        }
        ASSERT_STREQ(output.c_str(), "001102234334443563668778828");

        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(t1 - t0).count();
        Log::BioLog::echo("Res count: " + std::to_string(column.size()));
        Log::BioLog::echo(std::to_string(dur / 1000.0f) + " ms");
    }
}

TEST_F(ChunkReaderTest, ScanNodesChunkReaderTest) {
    auto access = _db->access();
    auto reader = access.getReader();
    ColumnNodes column;
    auto chunkReader = reader.scanNodes(&column);
    chunkReader.prepare();

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        while (chunkReader.getStatus() != ChunkReader::Status::Finished) {
            chunkReader.work();
        }

        std::string output;
        for (const auto& id : column) {
            output += std::to_string(id.getID());
        }
        ASSERT_STREQ(output.c_str(), "012345678");

        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(t1 - t0).count();
        Log::BioLog::echo("Res count: " + std::to_string(column.size()));
        Log::BioLog::echo(std::to_string(dur / 1000.0f) + " ms");
    }
}

TEST_F(ChunkReaderTest, GetEdgesChunkReaderTest) {
    auto access = _db->access();
    auto reader = access.getReader();

    ColumnEdges column;
    ColumnNodes inputNodeIDs({1, 3, 8});
    auto chunkReader = reader.getEdges(&column, &inputNodeIDs);
    chunkReader.prepare();

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        while (chunkReader.getStatus() != ChunkReader::Status::Finished) {
            chunkReader.work();
        }

        std::string output;
        for (const auto& v : column) {
            const auto& [source, target] = v.edgeDir == EdgeDirection::Incoming
                                             ? std::make_pair(v.otherID, v.nodeID)
                                             : std::make_pair(v.nodeID, v.otherID);
            output += std::to_string(v.edgeID.getID());
            output += std::to_string(source.getID());
            output += std::to_string(target.getID());
        }
        ASSERT_STREQ(output.c_str(), "234334001443668778828563");

        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(t1 - t0).count();
        Log::BioLog::echo("Res count: " + std::to_string(column.size()));
        Log::BioLog::echo(std::to_string(dur / 1000.0f) + " ms");
    }
}

TEST_F(ChunkReaderTest, GetInEdgesChunkReaderTest) {
    auto access = _db->access();
    auto reader = access.getReader();

    ColumnEdges column;
    ColumnNodes inputNodeIDs({1, 3, 8});
    auto chunkReader = reader.getInEdges(&column, &inputNodeIDs);
    chunkReader.prepare();

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        while (chunkReader.getStatus() != ChunkReader::Status::Finished) {
            chunkReader.work();
        }

        std::string output;
        for (const auto& v : column) {
            const auto& [source, target] = v.edgeDir == EdgeDirection::Incoming
                                             ? std::make_pair(v.otherID, v.nodeID)
                                             : std::make_pair(v.nodeID, v.otherID);
            output += std::to_string(v.edgeID.getID());
            output += std::to_string(source.getID());
            output += std::to_string(target.getID());
        }
        ASSERT_STREQ(output.c_str(), "001443668778828563");

        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(t1 - t0).count();
        Log::BioLog::echo("Res count: " + std::to_string(column.size()));
        Log::BioLog::echo(std::to_string(dur / 1000.0f) + " ms");
    }
}

TEST_F(ChunkReaderTest, GetOutEdgesChunkReaderTest) {
    auto access = _db->access();
    auto reader = access.getReader();

    ColumnEdges column;
    ColumnNodes inputNodeIDs({1, 2, 3, 8});
    auto chunkReader = reader.getOutEdges(&column, &inputNodeIDs);
    chunkReader.prepare();

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        while (chunkReader.getStatus() != ChunkReader::Status::Finished) {
            chunkReader.work();
        }

        std::string output;
        for (const auto& v : column) {
            const auto& [source, target] = v.edgeDir == EdgeDirection::Incoming
                                             ? std::make_pair(v.otherID, v.nodeID)
                                             : std::make_pair(v.nodeID, v.otherID);
            output += std::to_string(v.edgeID.getID());
            output += std::to_string(source.getID());
            output += std::to_string(target.getID());
        }
        ASSERT_STREQ(output.c_str(), "234334828");

        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = duration_cast<microseconds>(t1 - t0).count();
        Log::BioLog::echo("Res count: " + std::to_string(column.size()));
        Log::BioLog::echo(std::to_string(dur / 1000.0f) + " ms");
    }
}
