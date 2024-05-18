#include "TuringStart2Command.h"

#include <spdlog/spdlog.h>

#include "TuringToolConfig.h"
#include "ToolInit.h"
#include "Command.h"
#include "ProcessUtils.h"

namespace {

bool checkToolRunning(const std::string& toolName, const FileUtils::Path& pidFile) {
    if (FileUtils::exists(pidFile)) {
        std::string pidStr;
        if (FileUtils::readContent(pidFile, pidStr)) {
            spdlog::error("{} is already running in process {}", toolName, pidStr);
        }
        return true;
    }

    return false;
}

}

TuringStart2Command::TuringStart2Command(ToolInit& toolInit)
    : ToolCommand(toolInit),
    _startCommand("start2")
{
}

TuringStart2Command::~TuringStart2Command() {
}

void TuringStart2Command::setup() {
    auto& argParser = _toolInit.getArgParser();

    _startCommand.add_description("Start Turing platform with TuringDB v2");
    argParser.add_subparser(_startCommand);
}

bool TuringStart2Command::isActive() {
    const auto& argParser = _toolInit.getArgParser();
    return argParser.is_subcommand_used("start2");
}

void TuringStart2Command::run() {
    const auto turingAppDir = _toolInit.getOutputsDirPath()/TuringToolConfig::TURING_APP_DIR_NAME;
    const auto turingDBDir = _toolInit.getOutputsDirPath()/TuringToolConfig::TURING_DB_DIR_NAME;

    bool running = false;
    running = checkToolRunning("turing-app", turingAppDir/ProcessUtils::getPIDFileName());
    running |= checkToolRunning("turingdb", turingDBDir/ProcessUtils::getPIDFileName());
    if (running) {
        return;
    }

    _toolInit.createOutputDir();

    Command turingApp("turing-app");
    turingApp.addOption("-o", turingAppDir.string());
    turingApp.setWorkingDir(_toolInit.getOutputsDir());
    turingApp.setGenerateScript(true);
    turingApp.setWriteLogFile(false);
    turingApp.setScriptPath(_toolInit.getOutputsDirPath()/"turing-app.sh");
    turingApp.setLogFile(_toolInit.getOutputsDirPath()/"turing-app-launch.log");

    if (!turingApp.run()) {
        spdlog::error("Failed to start Turing application server");
        return;
    }
    
    const int appExitCode = turingApp.getReturnCode();
    if (appExitCode != 0) {
        spdlog::error("Failed to start turing-app: turing-app command terminated with exit code {}", appExitCode);
        return;
    }

    Command db("turingdb");
    db.addOption("-o", turingDBDir.string());
    db.setWorkingDir(_toolInit.getOutputsDir());
    db.setGenerateScript(true);
    db.setWriteLogFile(false);
    db.setScriptPath(_toolInit.getOutputsDirPath()/"turingdb.sh");
    db.setLogFile(_toolInit.getOutputsDirPath()/"turingdb-launch.log");

    if (!db.run()) {
        spdlog::error("Failed to start Turing database server");
        return;
    }

    const int dbExitCode = db.getReturnCode();
    if (dbExitCode != 0) {
        spdlog::error("Failed to start turingdb: turingdb command terminated with exit code {}", dbExitCode);
        return;
    }
}
