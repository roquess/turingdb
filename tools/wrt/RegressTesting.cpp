#include "RegressTesting.h"

#include <stdlib.h>
#include <sstream>
#include <string>

#include "FileUtils.h"
#include "Command.h"

#include "TimerStat.h"
#include "BioLog.h"
#include "MsgCommon.h"
#include "MsgWRT.h"

using namespace Log;

RegressTesting::RegressTesting(const Path& reportDir)
    : _reportDir(reportDir)
{
}

RegressTesting::~RegressTesting() {
}

void RegressTesting::run() {
    // Check that TURING_HOME exists
    const char* turingHome = getenv("TURING_HOME");
    if (!turingHome) {
        BioLog::log(msg::ERROR_INCORRECT_ENV_SETUP());
        return;
    }

    const auto currentDir = FileUtils::cwd();
    
    {
        TimerStat("Analyze tests");
        analyzeDir(currentDir);
        if (_error) {
            BioLog::echo("Some tests have errors, please fix test setup errors and retry.");
            return;
        }
    }

    BioLog::echo("\nTests detected: "+std::to_string(_testPaths.size())+"\n");

    runTests();

    writeTestResults();
}

void RegressTesting::clean() {
    TimerStat timerStat("Clean tests");
    const auto currentDir = FileUtils::cwd();

    BioLog::echo("Cleaning all tests");
    cleanDir(currentDir);
}

void RegressTesting::analyzeDir(const Path& dir) {
    const auto testListFile = dir/"test.list";
    if (!FileUtils::exists(testListFile)) {
        analyzeTest(dir);
        return;
    }

    std::string testList;
    if (!FileUtils::readContent(testListFile, testList)) {
        BioLog::log(msg::ERROR_FAILED_TO_OPEN_FOR_READ() << testListFile.string());
        _error = true;
        return;
    }

    std::istringstream f(testList);
    std::string line;
    Path testPath;
    while (std::getline(f, line)) {
        if (!line.empty()) {
            testPath = dir/line;
            if (!FileUtils::isDirectory(testPath)) {
                BioLog::log(msg::ERROR_DIRECTORY_NOT_EXISTS() << testPath.string());
                _error = true;
                continue;
            }

            analyzeDir(testPath);
        }
    }
}

void RegressTesting::analyzeTest(const Path& dir) {
    const auto runScriptPath = dir/"run.py";
    if (!FileUtils::exists(runScriptPath)) {
        BioLog::log(msg::ERROR_FILE_NOT_EXISTS() << runScriptPath.string());
        _error = true;
        return;
    }

    _testPaths.push_back(dir);
}

void RegressTesting::runTests() {
    TimerStat timerStat("Run tests");
    for (const auto& testPath : _testPaths) {
        runTest(testPath);
    }
}

void RegressTesting::runTest(const Path& dir) {
    const auto runScriptPath = dir/"run.py";
    if (!FileUtils::exists(runScriptPath)) {
        BioLog::log(msg::ERROR_FILE_NOT_EXISTS() << runScriptPath.string());
        _error = true;
        return;
    }

    Command cmd("python3");
    cmd.addArg(runScriptPath.string());
    cmd.setWorkingDir(dir);
    cmd.setLogFile(dir/"run.log");
    cmd.setGenerateScript(true);

    BioLog::echo("Run: "+dir.string());
    if (!cmd.run()) {
        BioLog::log(msg::ERROR_FAILED_TO_RUN_SCRIPT() << runScriptPath.string());
        BioLog::echo("Fail: "+dir.string());
        _testFail.push_back(dir);
        return;
    }

    const int res = cmd.getReturnCode();
    if (res == 0) {
        BioLog::echo("Pass: "+dir.string());
        _testSuccess.push_back(dir);
    } else {
        BioLog::echo("Fail: "+dir.string());
        _testFail.push_back(dir);
    }
}

void RegressTesting::writeTestResults() {
    // Tests summary
    std::string summary;
    summary += "Tests passed: " + std::to_string(_testSuccess.size()) + "\n";
    summary += "Tests failed: " + std::to_string(_testFail.size()) + "\n";
    summary += "Total tests detected: " + std::to_string(_testPaths.size()) + "\n";

    BioLog::echo("");
    BioLog::echo(summary);
    
    const auto testSumFile = _reportDir/"wrt.sum";
    if (!FileUtils::writeFile(testSumFile, summary)) {
        BioLog::log(msg::ERROR_FAILED_TO_WRITE_FILE() << testSumFile.string());
        return;
    }
}

void RegressTesting::cleanDir(const Path& dir) {
    if (!FileUtils::isDirectory(dir)) {
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        const auto entryPath = entry.path();
        if (FileUtils::isDirectory(entryPath)) {
            if (entryPath.extension() == ".out") {
                if (!FileUtils::removeDirectory(entryPath)) {
                    BioLog::log(msg::ERROR_FAILED_TO_REMOVE_DIRECTORY()
                                << entryPath.string());
                }
            }

            cleanDir(entryPath);
        } else {
            const auto name = entryPath.filename();
            if (name == "wrt.sum"
                || name == "run.log"
                || name == "cmd.sh") {

                if (!FileUtils::removeFile(entryPath)) {
                    BioLog::log(msg::ERROR_FAILED_TO_REMOVE_FILE()
                                << entryPath.string());
                }
            }
        }
    }
}
