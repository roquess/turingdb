#include "ToolInit.h"

#include <spdlog/spdlog.h>
#include <argparse.hpp>

#include "DBServer.h"
#include "DBServerConfig.h"

int main(int argc, const char** argv) {
    ToolInit toolInit("bioserver");

    auto& argParser = toolInit.getArgParser();
    argParser.add_argument("load")
             .help("Load a database at the start")
             .nargs(1);

    toolInit.init(argc, argv);

    const std::vector<std::string> dbNames = argParser.get<std::vector<std::string>>("-load");

    // Configuration of the DB Server
    DBServerConfig dbServerConfig;

    // Database server
    DBServer server(dbServerConfig);

    if (!server.run(dbNames)) {
        spdlog::error("Database server terminated with an error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
