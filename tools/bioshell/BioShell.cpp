#include "TuringClient.h"

#include "ToolInit.h"
#include "PerfStat.h"
#include "TimerStat.h"

#include "linenoise.h"

#include "BioLog.h"
#include "MsgShell.h"

using namespace turing::db::client;
using namespace Log;

int main(int argc, const char** argv) {
    ToolInit toolInit("bioshell");

    toolInit.init(argc, argv);

    TuringClient turing;
    if (!turing.connect()) {
        const auto& config = turing.getConfig();
        BioLog::log(msg::ERROR_SHELL_IMPOSSIBLE_TO_CONNECT() << config.getAddress() << config.getPort());

        BioLog::printSummary();
        BioLog::destroy();
        PerfStat::destroy();
        return EXIT_SUCCESS;
    }

    const char* shellPrompt = "turing> ";
    char* line = NULL;
    std::string lineStr;
    while ((line = linenoise(shellPrompt)) != NULL) {
        lineStr = line;
        if (lineStr.empty()) {
            continue;
        }

        if (!turing.executeQuery(lineStr)) {
            BioLog::log(msg::ERROR_SHELL_ERROR_DURING_QUERY_EXECUTION());
        }
        linenoiseHistoryAdd(line);
    }

    BioLog::printSummary();
    BioLog::destroy();
    PerfStat::destroy();
    return EXIT_SUCCESS;
}
