#include "ToolInit.h"

#include "BioLog.h"

#define BIOIMPORT_TOOL_NAME     "bioimport"

using namespace Log;

int main(int argc, const char** argv) {
    ToolInit toolInit(BIOIMPORT_TOOL_NAME);
    toolInit.init(argc, argv);

    BioLog::printSummary();
    BioLog::destroy();
    return EXIT_SUCCESS;
}
