#include <gtest/gtest.h>
#include <numeric>
#include <regex>
#include <span>
#include <thread>

#include "BioLog.h"
#include "DB.h"
#include "FileUtils.h"
#include "JobSystem.h"
#include "JsonParser.h"
#include "NodeView.h"
#include "PerfStat.h"
#include "Reader.h"
#include "Time.h"

using namespace db;

class JsonParserTest : public ::testing::Test {
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

TEST_F(JsonParserTest, General) {
    JobSystem jobSystem;
    jobSystem.initialize();
}
