#include "ToolInit.h"

#include "DBServer.h"
#include "DBServerConfig.h"

using namespace db;

int main(int argc, const char** argv) {
    ToolInit toolInit("turingdb");

    toolInit.init(argc, argv);

    DBServerConfig dbServerConfig;

    // Database server
    DBServer server(dbServerConfig);
    server.start();

    return EXIT_SUCCESS;
}
