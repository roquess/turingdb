#include "TimerStat.h"
#include "vmutils.h"

#include "Time.h"

using namespace db;

int main() {
    auto sample = VMSample::createSample(SAMPLE_NAME);

    if (!sample.loadJsonDB(sample._turingHome + "/neo4j/pole-db/")) {
        return 1;
    }

    auto t0 = Clock::now();
    std::vector<FileUtils::Path> sampleDirs = {
        sample._sampleDir + "/../vm-scanNodes",
        sample._sampleDir + "/../vm-scanEdges",
        sample._sampleDir + "/../vm-scanNodesByLabel",
        sample._sampleDir + "/../vm-scanOutEdgesByLabel",
        sample._sampleDir + "/../vm-scanInEdgesByLabel",
        sample._sampleDir + "/../vm-getInEdges",
        sample._sampleDir + "/../vm-getOutEdges",
        sample._sampleDir + "/../vm-complex",
        sample._sampleDir + "/../vm-sparseOutput",
        sample._sampleDir + "/../vm-filters",
        sample._sampleDir + "/../vm-scanNodeProperties",
        sample._sampleDir + "/../vm-scanNodePropertiesByLabel",
    };

    for (const auto& dir : sampleDirs) {
        TimerStat timer(dir);

        std::string dirName {dir.filename()};
        auto programPath = dir / "program.turing";
        spdlog::info("====== SAMPLE {} ======", dirName);
        if (!sample.executeFile(programPath.c_str())) {
            return 1;
        }
        if (!sample.executeFile(programPath.c_str())) {
            return 1;
        }
    }

    sample.destroy();

    spdlog::info("TOTAL Compilation + execution (x2) time: {} ms",
                 Milliseconds(Clock::now() - t0).count());
    return 0;
}
