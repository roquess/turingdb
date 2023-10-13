#include "RegressTesting.h"

#include "ToolInit.h"
#include "PerfStat.h"
#include "TimerStat.h"

#include "BioLog.h"

using namespace Log;

int main(int argc, const char** argv) {
    ToolInit toolInit("wrt");
    ArgParser& argParser = toolInit.getArgParser();
    argParser.addOption("clean", "Clean all test directories", false);

    toolInit.init(argc, argv);

    bool cleanDir = false;
    for (const auto& option : argParser.options()) {
        if (option.first == "clean") {
            cleanDir = true;
        }
    }

    RegressTesting regress(toolInit.getReportsDir());
    if (cleanDir) {
        regress.clean();
    } else {
        regress.run();
    }

    BioLog::printSummary();
    BioLog::destroy();
    PerfStat::destroy();
    return EXIT_SUCCESS;
}
