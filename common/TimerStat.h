#pragma once

#include <chrono>
#include <fstream>

class TimerStat {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using SecondsPeriod = std::chrono::seconds::period;
    using FloatSeconds = std::chrono::duration<float, SecondsPeriod>;

    TimerStat(const TimerStat& other) = delete;
    TimerStat(TimerStat&& other) = delete;
    TimerStat& operator=(const TimerStat& other) = delete;
    TimerStat& operator=(TimerStat&& other) = delete;

    TimerStat(std::ofstream* file);
    TimerStat(std::ofstream* file, const std::string& msg);
    ~TimerStat();

private:
    std::ofstream* _file;
    TimePoint _start;
    std::string _msg = "PerfStat";
};
