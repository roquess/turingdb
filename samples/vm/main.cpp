#include <bitset>
#include <spdlog/spdlog.h>

#include "Instruction.h"
#include "LogSetup.h"
#include "LogUtils.h"
#include "PerfStat.h"
#include "SystemManager.h"
#include "Time.h"
#include "TimerStat.h"
#include "VM.h"
#include "utils.h"

using namespace db;

[[gnu::noinline]] void execRawCpp(auto& registers, auto& system2) {
    registers[0] = system2->getDBCount();
    system2->createDB("DB");
    registers[2] = system2->getDBCount();
    auto* names = (std::vector<std::string_view>*)(registers[3]);
    system2->listDatabases(*names);
}

int main() {
    LogSetup::setupLogFileBacked(SAMPLE_NAME ".log");
    PerfStat::init(SAMPLE_NAME ".perf");

    // Initialize system
    auto system = std::make_unique<SystemManager>();
    auto system2 = std::make_unique<SystemManager>();

    // Initialize VM
    VM vm(system.get());

    // Generate byte code
    uint8_t reg1 = 1;
    uint8_t reg3 = 3;

    std::vector<std::string_view> names;
    std::vector<std::string_view> names2;
    vm.setRegisterValue(reg1, 8);
    vm.setRegisterValue(reg3, reinterpret_cast<uint64_t>(&names));
    std::array<uint64_t, 256> registers {};
    registers[1] = 8;
    registers[3] = reinterpret_cast<uint64_t>(&names2);

    {
        TimerStat timer {"Raw C++"};

        auto t0 = Clock::now();
        for (size_t i = 0; i < 50000000; i++) {
            names2.clear();
            execRawCpp(registers, system2);
        }
        auto t1 = Clock::now();
        spdlog::info("-- Raw C++ program");
        logt::ElapsedTime(duration<Milliseconds>(t0, t1), "ms");
    }

    {
        TimerStat timer {"VM"};
        auto t0 = Clock::now();
        for (size_t i = 0; i < 50000000; i++) {
            names.clear();

            // Execute instructions
            vm.exec(getTestProgram());
        }
        auto t1 = Clock::now();
        spdlog::info("-- Pointer to go to implementation");
        logt::ElapsedTime(duration<Milliseconds>(t0, t1), "ms");
    }


    spdlog::info("DB Names:");
    for (auto name : names) {
        spdlog::info(" - {}", name);
    }

    PerfStat::destroy();
    return 0;
}
