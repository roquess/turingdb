#include <stdlib.h>
#include <string.h>
#include <memory>
#include <signal.h>

#include <spdlog/spdlog.h>
#include <argparse.hpp>

#include "ToolInit.h"
#include "BannerDisplay.h"

#include "TuringServer.h"

using namespace app;

// This is necessary to handle unix signals
std::unique_ptr<TuringServer> server;

void signalHandler(int signum) {
    if (server) {
        server->terminate();
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, const char** argv) {
    BannerDisplay::printBanner();

    ToolInit toolInit("turing-app");

    bool isPrototypeMode = false;

    // Arguments parsing
    auto& argParser = toolInit.getArgParser();
    argParser.add_argument("--prototype")
             .store_into(isPrototypeMode);

    toolInit.init(argc, argv);

    bool isDevMode = false;
    {
        const char* turingDev = getenv("TURING_DEV");
        if (turingDev) {
            isDevMode = strcmp(turingDev, "1");
        }
    }

    server = std::make_unique<TuringServer>(toolInit.getOutputsDir());
    server->setPrototypeMode(isPrototypeMode);
    server->setDevMode(isDevMode);

    // Install signal handler to handle ctrl+C
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    server->start();

    // Wait for termination
    const auto serverType = server->waitServerDone();
    const int code = server->getReturnCode(serverType);

    std::string output;
    server->getOutput(serverType, output);

    spdlog::info("Server {} terminated with exit code {}\n{}",
                 server->getServerName(serverType),
                 code,
                 output);

    return EXIT_SUCCESS;
}
