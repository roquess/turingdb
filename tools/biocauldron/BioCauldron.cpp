#include "ToolInit.h"

#include "TuringUIServer.h"
#include "BioLog.h"
#include "MsgUIServer.h"

#define BIOCAULDRON_TOOL_NAME "biocauldron"

using namespace ui;
using namespace Log;

int main(int argc, const char** argv) {
    ToolInit toolInit(BIOCAULDRON_TOOL_NAME);

    ArgParser& argParser = toolInit.getArgParser();
    argParser.addOption("dev",
                        "Use a developpment environment instead of production",
                        false);
    toolInit.init(argc, argv);

    TuringUIServer server {toolInit.getOutputsDir()};

    argParser.isOptionSet("dev")
                 ? server.startDev()
                 : server.start();

    server.wait();
    const int code = server.getReturnCode();

    if (code != 0) {
        BioLog::printSummary();
        BioLog::destroy();
        return EXIT_FAILURE;
    }

    BioLog::printSummary();
    BioLog::destroy();
    return EXIT_SUCCESS;
}
