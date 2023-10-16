#pragma once

#include <filesystem>
#include <vector>
#include <queue>
#include <list>

#include "BoostProcess.h"

class RegressTesting {
public:
    using Path = std::filesystem::path;

    RegressTesting(const Path& reportPath);
    ~RegressTesting();

    void run();
    void clean();

private:
    const Path _reportDir;
    size_t _concurrency {1};
    bool _error {false};
    ProcessGroup _processGroup;
    std::vector<Path> _testPaths;
    std::vector<Path> _testFail;
    std::vector<Path> _testSuccess;
    std::queue<Path> _testWaitQueue;
    std::list<ProcessChild> _runningTests;

    void analyzeDir(const Path& dir);
    void analyzeTest(const Path& dir);
    void runTests();
    ProcessChild runTest(const Path& dir);
    void writeTestResults();
    void cleanDir(const Path& dir);
};
