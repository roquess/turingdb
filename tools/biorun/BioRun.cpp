#include "ToolInit.h"

#include "NotebookRunner.h"

#include "BioLog.h"

using namespace Log;

int main(int argc, const char** argv) {
    ToolInit toolInit("biorun");
    ArgParser& argParser = toolInit.getArgParser();
    argParser.setArgsDesc("notebook.ipynb ...");
    argParser.addOption("q", "Launch the notebooks in quiet mode.", false);

    toolInit.init(argc, argv);

    bool quiet = false;
    for (const auto& option : argParser.options()) {
        if (option.first == "q") {
            quiet = true;
        }
    }

    // Run notebooks
    NotebookRunner notebookRunner(toolInit.getOutputsDir());
    notebookRunner.setQuiet(quiet);
    for (const auto& arg : argParser.args()) {
        notebookRunner.addNotebook(arg);
    }

    notebookRunner.run();

    BioLog::printSummary();
    BioLog::destroy();
    return EXIT_SUCCESS;
}
