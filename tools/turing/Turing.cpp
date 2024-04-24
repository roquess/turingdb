#include <stdlib.h>
#include <string.h>

#include <spdlog/spdlog.h>
#include <argparse.hpp>

#include "ToolInit.h"

int main(int argc, const char** argv) {
    ToolInit toolInit("turing");
    toolInit.disableOutputDir();

    bool isPrototypeMode = false;

    // Arguments parsing
    auto& argParser = toolInit.getArgParser();

    argparse::ArgumentParser serverCommand("server");
    serverCommand.add_description("Manage Turing server");

    // turing server start
    argparse::ArgumentParser serverStartCommand("start");
    serverStartCommand.add_description("Start Turing server");

    serverStartCommand.add_argument("--prototype")
                      .store_into(isPrototypeMode);

    serverCommand.add_subparser(serverStartCommand);

    // turing server stop
    argparse::ArgumentParser serverStopCommand("stop");
    serverStopCommand.add_description("Stop Turing server");
    serverCommand.add_subparser(serverStopCommand);

    argParser.add_subparser(serverCommand);

    toolInit.init(argc, argv);

    if (argParser.is_subcommand_used(serverCommand)) {
        toolInit.createOutputDir();
    }

    /*
    const bool startRequested = serverCommand.is_subcommand_used(serverStartCommand);
    const bool stopRequested = serverCommand.is_subcommand_used(serverStopCommand);

    bool isDevMode = false;
    {
        const char* turingDev = getenv("TURING_DEV");
        if (turingDev) {
            isDevMode = strcmp(turingDev, "1");
        }
    }
    */

    return EXIT_SUCCESS;
}
