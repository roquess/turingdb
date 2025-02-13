#include <stdlib.h>
#include <argparse.hpp>

#include "TuringDB.h"
#include "LocalMemory.h"

#include "ToolInit.h"
#include "TuringShell.h"

using namespace db;

int main(int argc, const char** argv) {
    ToolInit toolInit("turingdb");

    auto& argParser = toolInit.getArgParser();

    bool noServer = false;
    argParser.add_argument("-noserver")
             .store_into(noServer);

    toolInit.init(argc, argv);

    TuringDB turingDB;
    LocalMemory mem;

    if (noServer) {
        TuringShell shell(turingDB, &mem);
        shell.startLoop();
    } else {
    }

    return EXIT_SUCCESS;
}

