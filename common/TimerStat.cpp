#include "TimerStat.h"
#include "FileUtils.h"

using namespace std::literals;
using Clock = std::chrono::system_clock;
using FloatDuration = std::chrono::duration<float>;

TimerStat::TimerStat(std::ofstream* file)
    : _file(file),
      _start(Clock::now())
{
}

TimerStat::TimerStat(std::ofstream* file, const std::string& msg)
    : _file(file),
      _start(Clock::now()),
      _msg(msg)
{
}

TimerStat::~TimerStat() {
    if (_file->is_open()) {
        auto duration = Clock::now() - _start;
        float v = std::chrono::duration_cast<FloatSeconds>(duration).count();

        *_file << '[' << _msg << "] Elapsed time: " << std::to_string(v)
               << " s\n";
    }
}
