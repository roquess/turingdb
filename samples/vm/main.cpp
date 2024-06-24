#include <bitset>
#include <nmmintrin.h>
#include <range/v3/view/join.hpp>
#include <span>
#include <spdlog/spdlog.h>

#include "Compiler.h"
#include "LogSetup.h"
#include "LogUtils.h"
#include "Neo4j/ParserConfig.h"
#include "Neo4jImporter.h"
#include "PerfStat.h"
#include "Program.h"
#include "SystemManager.h"
#include "Time.h"
#include "VM.h"
#include "utils.h"

using namespace db;

__attribute__((noinline)) void getSum(uint64_t* sum,
                                      const uint64_t* count,
                                      uint64_t* i) {
    uint64_t ii = *i;
    uint64_t c  = *count;
    uint64_t s  = *sum;
    for (ii = 0; ii < c; ii++) {
        s += ii;
    }
    *sum = s;
}

int main() {
    LogSetup::setupLogFileBacked(SAMPLE_NAME ".log");
    PerfStat::init(SAMPLE_NAME ".perf");
    spdlog::set_level(spdlog::level::info);
    const std::string turingHome = std::getenv("TURING_HOME");
    JobSystem jobSystem;
    jobSystem.initialize();

    // Create compiler
    auto compiler = Compiler::create();

    // Initialize system
    auto system = std::make_unique<SystemManager>();

    //// Load pole db
    ////Neo4jImporter::importJsonDir(
    ////    jobSystem,
    ////    system->getDefaultDB(),
    ////    nodeCountLimit,
    ////    edgeCountLimit,
    ////    Neo4jImporter::ImportJsonDirArgs {
    ////        ._jsonDir = turingHome + "/neo4j/pole-db/",
    ////    });

    // Initialize VM
    VM vm(system.get());

    // Compile & execute program
    auto t0 = Clock::now();
    spdlog::info("== Compilation ==");
    Program program = compiler->compileFile(turingHome + "/samples/vm/data.turing");
    if (program.size() == 0) {
        return 1;
    }
    logt::ElapsedTime(Microseconds(Clock::now() - t0).count(), "us");

    // Init VM
    t0 = Clock::now();
    spdlog::info("== Init VM ==");
    vm.initialize();
    logt::ElapsedTime(Microseconds(Clock::now() - t0).count(), "us");

    // Execution
    t0 = Clock::now();
    spdlog::info("== Execution ==");
    vm.exec(program);
    const float vmTime = Milliseconds(Clock::now() - t0).count();
    logt::ElapsedTime(vmTime, "ms");
    spdlog::info("Reg0: {}", vm.readRegister(0));
    spdlog::info("Reg1: {}", vm.readRegister(1));
    spdlog::info("Reg2: {}", vm.readRegister(2));
    spdlog::info("Reg3: {}", vm.readRegister(3));
    spdlog::info("Reg4: {}", vm.readRegister(4));

    // auto countStart = vm.readRegister(0);
    // auto countEnd = vm.readRegister(1);
    // spdlog::info("DB count at the beginning: {}", countStart);
    // spdlog::info("DB count at the end {}", countEnd);

    spdlog::info("== Pure C++ ==");
    t0 = Clock::now();
    uint64_t sum = 0;
    uint64_t i = 0;
    const uint64_t count = getCount();
    getSum(&sum, &count, &i);
    spdlog::info("Sum:  {}", sum);

    const float cppTime = Milliseconds(Clock::now() - t0).count();
    logt::ElapsedTime(cppTime, "ms");
    spdlog::info("C++ is faster than the VM by: {}", vmTime / cppTime);

    PerfStat::destroy();
    return 0;
}
