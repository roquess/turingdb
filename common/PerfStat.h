#pragma once

#include "TimerStat.h"
#include <fstream>
#include <filesystem>
#include <string>

class TimerStat;

class PerfStat {
public:
    using Path = std::filesystem::path;

    PerfStat();

    static void init(const Path& path);

    static PerfStat* getInstance();
    [[nodiscard]] static TimerStat spawn();
    [[nodiscard]] static TimerStat spawn(const std::string& msg);

    static void destroy();

private:
    std::ofstream _outStream;
    static PerfStat* _instance;

    void open(const Path& path);
    void close();
};
