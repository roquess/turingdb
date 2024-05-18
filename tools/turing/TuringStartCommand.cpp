#include "TuringStartCommand.h"

#include <spdlog/spdlog.h>

#include "TuringToolConfig.h"
#include "ToolInit.h"
#include "Command.h"
#include "ProcessUtils.h"
#include "StringToNumber.h"

namespace {

bool checkToolRunning(const std::string& toolName, const FileUtils::Path& pidFile) {
    if (FileUtils::exists(pidFile)) {
        std::string pidStr;
        if (FileUtils::readContent(pidFile, pidStr)) {
            bool convertError = false;
            const pid_t pid = StringToNumber<pid_t>(pidStr, convertError);
            if (convertError) {
                return false;
            }

            if (ProcessUtils::isProcessRunning(pid)) {
                spdlog::error("{} is already running in process {}", toolName, pidStr);
                return true;
            }
        }
    }

    return false;
}

}

TuringStartCommand::TuringStartCommand(ToolInit& toolInit)
    : ToolCommand(toolInit),
    _startCommand("start")
{
}

TuringStartCommand::~TuringStartCommand() {
}

void TuringStartCommand::setup() {
    auto& argParser = _toolInit.getArgParser();

    _startCommand.add_description("Start Turing platform");
    _startCommand.add_argument("-db")
                 .metavar("dbname")
                 .default_value("reactome")
                 .nargs(1);
    argParser.add_subparser(_startCommand);
}

bool TuringStartCommand::isActive() {
    const auto& argParser = _toolInit.getArgParser();
    return argParser.is_subcommand_used("start");
}

void TuringStartCommand::run() {
    const auto turingAppDir = _toolInit.getOutputsDirPath()/TuringToolConfig::TURING_APP_DIR_NAME;
    const auto bioServerDir = _toolInit.getOutputsDirPath()/TuringToolConfig::BIOSERVER_DIR_NAME;

    if (checkToolRunning("turing-app", turingAppDir/ProcessUtils::getPIDFileName())) {
        spdlog::info("Turing application server turing-app is already running");
        return;
    }
    
    if (checkToolRunning("bioserver", bioServerDir/ProcessUtils::getPIDFileName())) {
        spdlog::info("Turing bioserver is already running");
        return;
    }

    _toolInit.createOutputDir();
    const auto dbName = getDBName();

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

    Command db("bioserver");
    db.addOption("-o", bioServerDir.string());
    db.addOption("-db", dbName);
    db.setWorkingDir(_toolInit.getOutputsDir());
    db.setGenerateScript(true);
    db.setWriteLogFile(false);
    db.setScriptPath(_toolInit.getOutputsDirPath()/"bioserver.sh");
    db.setLogFile(_toolInit.getOutputsDirPath()/"bioserver-launch.log");

    if (!db.run()) {
        spdlog::error("Failed to start Turing database server");
        return;
    }

    const int dbExitCode = db.getReturnCode();
    if (dbExitCode != 0) {
        spdlog::error("Failed to start bioserver: bioserver command terminated with exit code {}", dbExitCode);
        return;
    }
}

std::string TuringStartCommand::getDBName() {
    auto& argParser = _toolInit.getArgParser();
    auto& startCommand = argParser.at<argparse::ArgumentParser>("start");
    const std::string& dbName = startCommand.get<std::string>("-db");
    return dbName;
}
