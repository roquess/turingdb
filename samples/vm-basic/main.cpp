#include <spdlog/spdlog.h>

#include "Assembler.h"
#include "DataEnv.h"
#include "LogSetup.h"
#include "LogUtils.h"
#include "PerfStat.h"
#include "Program.h"
#include "SystemManager.h"
#include "Time.h"
#include "VM.h"

using namespace db;

int main() {
    LogSetup::setupLogFileBacked(SAMPLE_NAME ".log");
    PerfStat::init(SAMPLE_NAME ".perf");
    spdlog::set_level(spdlog::level::info);
    const std::string turingHome = std::getenv("TURING_HOME");
    const std::string sampleDir = turingHome + "/samples/" SAMPLE_NAME;
    Program program;

    // Create assembler
    Assembler assembler;

    // Initialize system
    auto system = std::make_unique<SystemManager>();

    // Initialize VM
    VM vm(system.get());


    // Assemble program
    spdlog::info("== Assembly ==");
    auto t0 = Clock::now();

    if (!assembler.generateFromFile(program, sampleDir + "/program.turing")) {
        spdlog::error("Error program invalid");
        return 1;
    }

    logt::ElapsedTime(Microseconds(Clock::now() - t0).count(), "us");

    // Initialize VM
    spdlog::info("== Init VM ==");
    t0 = Clock::now();

    vm.initialize();
    logt::ElapsedTime(Microseconds(Clock::now() - t0).count(), "us");


    // Execution
    spdlog::info("== Execution ==");
    t0 = Clock::now();

    vm.exec(&program);
    spdlog::info("Sum: {}", vm.readRegister(0));
    logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");

    PerfStat::destroy();
    return 0;
}
