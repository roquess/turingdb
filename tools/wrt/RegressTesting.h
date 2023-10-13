#pragma once

#include <filesystem>
#include <vector>

class RegressTesting {
public:
    using Path = std::filesystem::path;

    RegressTesting(const Path& reportPath);
    ~RegressTesting();

    void run();
    void clean();

private:
    const Path _reportDir;
    bool _error {false};
    std::vector<Path> _testPaths;
    std::vector<Path> _testFail;
    std::vector<Path> _testSuccess;

    void analyzeDir(const Path& dir);
    void analyzeTest(const Path& dir);
    void runTests();
    void runTest(const Path& dir);
    void writeTestResults();
    void cleanDir(const Path& dir);
};
