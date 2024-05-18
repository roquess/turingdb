#include <signal.h>

#include <spdlog/spdlog.h>
#include <argparse.hpp>

#include "ToolInit.h"
#include "DBServer.h"
#include "DBServerConfig.h"
#include "Demonology.h"
#include "ProcessUtils.h"

void signalHandler(int signum) {
    spdlog::info("Server received signal {}, terminating", signum);
    exit(EXIT_SUCCESS);
}

int main(int argc, const char** argv) {
    ToolInit toolInit("bioserver");

    std::vector<std::string> dbNames;

    auto& argParser = toolInit.getArgParser();
    argParser.add_argument("-db")
             .help("Load a database at the start")
             .nargs(1)
             .append()
             .metavar("dbname")
             .store_into(dbNames);

    toolInit.init(argc, argv);

    // Demonize
    Demonology::demonize();

    // Write PID file
    const auto pidFilePath = toolInit.getOutputsDirPath()/ProcessUtils::getPIDFileName();
    if (!ProcessUtils::writePIDFile(pidFilePath)) {
        spdlog::error("Failed to write PID file {}", pidFilePath.string());
        exit(EXIT_FAILURE);
    }

    // Install signal handler to handle ctrl+C
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Configuration of the DB Server
    DBServerConfig dbServerConfig;

    // Database server
    DBServer server(dbServerConfig);

    spdlog::info("Server starting");
    if (!server.run(dbNames)) {
        spdlog::error("Database server terminated with an error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
